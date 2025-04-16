import serial
import pygame
import time

def play_audio_with_serial(audio_file, port='COM5', baud_rate=9600):
    # Initialize pygame mixer
    pygame.mixer.init()
    
    # Initialize serial connection once at the beginning
    ser = serial.Serial(port, baud_rate)
    time.sleep(2)  # Wait for Arduino to reset
    
    try:
        while True:
            # Wait for user to press Enter to start
            input("Press Enter to start playing...")
            
            try:
                # Load and play the audio file
                pygame.mixer.music.load(audio_file)
                
                # Send start signal to Arduino
                ser.write(b'START\n')
                
                # Add delay before starting music
                time.sleep(0.2)  # 200ms delay
                
                # Play the audio
                pygame.mixer.music.play()
                
                # Wait for the audio to finish
                while pygame.mixer.music.get_busy():
                    time.sleep(0.1)
                
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                # Try to reconnect
                try:
                    ser.close()
                    time.sleep(1)
                    ser = serial.Serial(port, baud_rate)
                    time.sleep(2)
                except:
                    print("Failed to reconnect to serial port")
                    break
            
            # Ask if user wants to play again
            user_input = input("Press Enter to play again, or 'q' to quit: ")
            if user_input.lower() == 'q':
                break
            
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        # Clean up
        ser.close()
        pygame.mixer.quit()

if __name__ == "__main__":
    # Replace 'your_audio_file.mp3' with your actual audio file path
    audio_file = "prototype glow sound.mp3"
    # Replace 'COM3' with your actual Arduino port
    play_audio_with_serial(audio_file, port='COM5') 