import serial
import json
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from collections import deque
import time

# Serial port and baud rate
SERIAL_PORT = '/dev/ttyUSB0'  # replace with your port
BAUD_RATE = 230400

# Number of samples to keep for trail visualization
MAX_SAMPLES = 100

# Deques for 3D plotting
acc_data = deque(maxlen=MAX_SAMPLES)
gyr_data = deque(maxlen=MAX_SAMPLES)
mag_data = deque(maxlen=MAX_SAMPLES)

# Setup serial
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# Setup interactive 3D plot
plt.ion()
fig = plt.figure(figsize=(18, 6))

# Create subplots with different viewing angles
ax1 = fig.add_subplot(141, projection='3d')
ax2 = fig.add_subplot(142, projection='3d')
ax3 = fig.add_subplot(143, projection='3d')
ax4 = fig.add_subplot(144, projection='3d')  # Combined view

# Set initial viewing angles for better perspective
ax1.view_init(elev=20, azim=45)
ax2.view_init(elev=20, azim=45)  
ax3.view_init(elev=20, azim=45)
ax4.view_init(elev=20, azim=45)

def setup_3d_axis(ax, title, limit, color='black'):
    ax.set_xlabel('X', color=color)
    ax.set_ylabel('Y', color=color)
    ax.set_zlabel('Z', color=color)
    ax.set_title(title, color=color, fontsize=12, fontweight='bold')
    ax.set_xlim([-limit, limit])
    ax.set_ylim([-limit, limit]) 
    ax.set_zlim([-limit, limit])
    
    # Add coordinate system reference
    ax.quiver(0, 0, 0, limit*0.3, 0, 0, color='gray', alpha=0.3, arrow_length_ratio=0.1)
    ax.quiver(0, 0, 0, 0, limit*0.3, 0, color='gray', alpha=0.3, arrow_length_ratio=0.1)
    ax.quiver(0, 0, 0, 0, 0, limit*0.3, color='gray', alpha=0.3, arrow_length_ratio=0.1)

def draw_sensor_data(ax, data_deque, color, title, limit, scale=1.0):
    if not data_deque:
        return
    
    current_data = data_deque[-1]
    scaled_data = [x * scale for x in current_data]
    
    # Current vector (thick bright arrow)
    ax.quiver(0, 0, 0, scaled_data[0], scaled_data[1], scaled_data[2], 
              color=color, arrow_length_ratio=0.1, linewidth=4, alpha=1.0)
    
    # Draw trail as connected line
    if len(data_deque) > 1:
        trail_x = [data[0] * scale for data in data_deque]
        trail_y = [data[1] * scale for data in data_deque] 
        trail_z = [data[2] * scale for data in data_deque]
        ax.plot(trail_x, trail_y, trail_z, color=color, alpha=0.3, linewidth=1)
        
        # Recent points as dots
        for i in range(max(0, len(data_deque)-10), len(data_deque)):
            alpha = (i + 1) / len(data_deque)
            data_point = [data_deque[i][j] * scale for j in range(3)]
            ax.scatter(data_point[0], data_point[1], data_point[2], 
                      color=color, alpha=alpha*0.6, s=20)
    
    # Add magnitude and individual component text
    magnitude = np.linalg.norm(current_data)
    info_text = f'{title}\nMag: {magnitude:.1f}\nX: {current_data[0]:.1f}\nY: {current_data[1]:.1f}\nZ: {current_data[2]:.1f}'
    ax.text2D(0.02, 0.98, info_text, transform=ax.transAxes, 
              verticalalignment='top', fontsize=8, 
              bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.8))

print("Starting Enhanced 3D IMU Visualization...")
print("• Red = Accelerometer (mg)")  
print("• Blue = Gyroscope (°/s)")
print("• Green = Magnetometer (μT)")
print("• Combined view shows all sensors")
print("Press Ctrl+C to exit")

frame_count = 0
start_time = time.time()

try:
    while True:
        try:
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue
            
            # Parse JSON
            data = json.loads(line)
            
            # Store data as 3D vectors
            acc_vector = [data['accX'], data['accY'], data['accZ']]
            gyr_vector = [data['gyrX'], data['gyrY'], data['gyrZ']]  
            mag_vector = [data['magX'], data['magY'], data['magZ']]
            
            acc_data.append(acc_vector)
            gyr_data.append(gyr_vector)
            mag_data.append(mag_vector)
            
            # Clear all plots
            for ax in [ax1, ax2, ax3, ax4]:
                ax.clear()
            
            # Plot individual sensors
            setup_3d_axis(ax1, "Accelerometer", 1000, 'red')
            draw_sensor_data(ax1, acc_data, 'red', 'ACC', 1000, scale=1.0)
            
            setup_3d_axis(ax2, "Gyroscope", 10, 'blue') 
            draw_sensor_data(ax2, gyr_data, 'blue', 'GYR', 10, scale=1.0)
            
            setup_3d_axis(ax3, "Magnetometer", 100, 'green')
            draw_sensor_data(ax3, mag_data, 'green', 'MAG', 100, scale=1.0)
            
            # Combined view with scaled vectors
            setup_3d_axis(ax4, "Combined View", 100, 'black')
            draw_sensor_data(ax4, acc_data, 'red', 'ACC', 100, scale=0.1)      # Scale down acc
            draw_sensor_data(ax4, gyr_data, 'blue', 'GYR', 100, scale=10)      # Scale up gyr  
            draw_sensor_data(ax4, mag_data, 'green', 'MAG', 100, scale=1.0)    # Keep mag same
            
            # Calculate and display frame rate
            frame_count += 1
            if frame_count % 50 == 0:
                elapsed = time.time() - start_time
                fps = frame_count / elapsed
                fig.suptitle(f'IMU 3D Visualization - {fps:.1f} FPS', fontsize=14, fontweight='bold')
            
            plt.tight_layout()
            plt.pause(0.001)  # Smaller pause for smoother animation
            
        except json.JSONDecodeError:
            print("Invalid JSON:", line)
            continue
        except Exception as e:
            print(f"Error: {e}")
            continue

except KeyboardInterrupt:
    print("\nStopping visualization...")

finally:
    ser.close()
    plt.close('all')
    print("Serial port closed.")