# funny-voices
Use a microcontroller to alter a voice recording

## Overview

This project uses an ESP32 based microcontroller with an integrated touch screen, microphone, and speaker. The project allows the user to record a message and then playback at a much slower or faster speed to change their voice.

## Background

I have a collection of microcontrollers and sensors. I am working my way through the collection of equipment I have and looking at fun projects I can put together, and also expand my knowledge base. 

## What did I use?

Here’s a picture of the components I used for this project:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/985749c4-b4bc-4c13-8540-97c4657b5a2e" />

The Core2 microcontroller has an integrated touch screen, microphone, and speaker.

## What does it look like?

This is what the project looks like when it’s running:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/052d8e69-91f7-4775-a575-aaacb9baba31" />

The project displays the current playback speed, at the top of the screen, which can be slow, normal or fast.

There are 4 buttons at the bottom of the screen which reduce the playback speed (<), increase the playback speed (>), start recording (+) and stop recording (-). When the user clicks record (+), the screen will be updated with a large red circle:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/fbb1ad19-658c-4c6c-bd42-c7978f9342f0" />

To stop recording, the user presses the - button:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/72f2767b-b0e7-4dd9-90c7-c2e238f4d9b2" />

The screen will update with a green circle to indicate recording has stopped and playback has started. Once a recording has completed, the playback button can be pressed to replay recording as many times as the user would like. They can select different speeds between playback cycles.

The Core2 can only record or playback – it cannot do both at once. The following screen is displayed to tell the user to wait for recording or playback to complete before continuing with their request:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/69d6a06e-6dd3-4c6f-8f09-cad98c5cb3fc" />

Here's a video showing the project working:

[![short video](https://img.youtube.com/vi/BeuMcezUqzY/0.jpg)](https://www.youtube.com/watch?v=BeuMcezUqzY)

## Technical Overview

Here’s an overview of the recording/playback process:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/3075e8c2-d7c8-4a61-b232-a1288e72d80c" />

The project uses an Arduino Sketch to initialize the microcontroller display, speaker, and microphone as well as output the action buttons.

The microcontroller’s sound recording library uses WAV format data. The best results are achieved using 16KHz 16-bit mono recordings. Recording always occurs using these settings.

The playback can be at 16KHz which will playback the user’s normal voice. When slowed down, the playback occurs at 12KHz which makes the voice deeper and slower. This is 75% of the original recording speed. Running at 8KHz is possible, but it’s hard to tell what the recording is saying. When sped up, the playback occurs at 32KHz which makes the voice much higher and deeper. No other recording attributes have to be changed to create these effects.

The one-second recording buffer is enough time for small snippets. This can be adjusted by changing the recordSeconds variable in the Sketch.

The microcontroller can only record or play sounds. It cannot do both at the same time. There is some application state to track when the Sketch is recording or playing back. There is also code to wait for the microphone to stop recording or the speaker to finish playing before allowing the next record/playback cycle.

## What do I need?

You will need:

1. The project uses a Core2 AWS from M5Stack.
       
2. The application only uses the standard M5Stack and Core2 libraries. 
       
3. A PC with Windows, Linux, or a Mac to install the Arduino IDE which can be downloaded here https://www.arduino.cc/en/software/. 
       
4. A USB A to USB C cable to connect the PC or Mac to the Core2.
       
5. The git utility to access the GIT repository (git clone https://github.com/davygotgit/headsup.git) or visit https://github.com/davygotgit/headsup and download a ZIP file.
  
Information for Core2 AWS is here https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit-for-aws-iot-edukit.

The main M5Stack site is here https://m5stack.com/.

This link contained basic information to be able to create this project https://github.com/m5stack/M5Unified/tree/master/examples/Basic/Microphone. 

## How do I install and configure the tools?

Here are some instructions for downloading and installing GIT https://github.com/git-guides/install-git.

Here are some instructions on how to download and install the Arduino IDE https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/.

M5Stack have an excellent quick start guide here https://docs.m5stack.com/en/arduino/m5core2/program. I don’t recall having to install any driver on my version of Ubuntu 24.04.2 LTS. It’s possible this is already included in the kernel. On Linux you must add your account to the dialout group by running the following bash command:

	sudo usermod -a -G dialout <your_account>

For example, if your user account is fantasticfred:

	sudo usermod -a -G dialout  fantasticfred

You must log out your current session and log back in again for this change to become active.

For Linux systems, I would first see if your Arduino IDE can see the Core2 device before attempting to install any drivers.

You know you are connected to the Core2 if you see something similar to the following status (bottom right) in the Arduino IDE:

<img width="519" height="67" alt="image" src="https://github.com/user-attachments/assets/221912df-61ea-4cf4-ba89-99220ef82b5c" />

## How do I build and install the application?

You need to download the code from the GIT repository. This can be done by visiting https://github.com/davygotgit/funny-voices and downloading, and then extracting, a ZIP file or by running the following terminal command from bash, a Windows Command Prompt or any suitable GIT access tool:

	git clone https://github.com/davygotgit/funny-voices.git

There are a couple of options to build the application for the first time. Option 1 is:

1. Start the Arduino IDE.
2. Create a new project using the File -> New Sketch menu option.
3. Save the project using the name funnyvoices by using the File -> Save menu option.
4. Open the src/funnyvoices .ino file, from repository, using another editor, and copy/paste the contents over the skeleton project.
       
Option 2 is:
       
1. Start the Arduino IDE.
2. Create a new project using the File -> New Sketch menu option.
3. Save the project using the name funnyvoices by using the File -> Save menu option.
4. Use the Sketch -> Show Sketch Folder menu option to get the location of the project (Sketch location). This will be similar to Home/Arduino/funnyvoices on Linux.
5. Close the IDE.
6. Copy the funnyvoices.ino file from the src subdirectory of the repository to the Sketch location.
7. Start the Arduino IDE and load the funnyvoices project.
       
Once you have the initial project saved, you can just load it from File -> Open Recent menu option.

With the Sketch loaded, connect the Core2 using the USB A to USB C cable,. Ensure the M5Core2 board is selected and the USB port shows a connected status. Press the Upload button on the toolbar. The Sketch will be compiled and transferred to the Core2. The application will start after the transfer completes.

Do you have any tips for using M5Stack microcontrollers, sensors, or units?

The majority of M5Stack controllers have library on github. The Core2 library is here https://github.com/m5stack/M5Core2?tab=readme-ov-file. The libraries usually have basic and advance samples to give you an idea of how to initialize and then work with the integrated devices like the screen, speaker, microphone, or IMU (accelerometer). The examples can also be found using File -> Examples from the Arduino IDE.

Over the past couple of years, M5Stack have been moving to a unified set of libraries and header files. You can find additional examples here https://github.com/m5stack/M5Unified.

All the examples will give you the confidence that a particular microcontroller and component e.g. IMU are working before you add more sensors and your own code. This will significantly cut down on your debug time as problems may arise from interaction between multiple sensors or your own code.

## Were there any challenges creating this project?

I ran into two challenges with this project.

The first challenge was with the sizing of the recording buffer. I thought I had sized the buffer correctly by using samples/sec * channels (mono == 1) * sample size (16 bit) * seconds, which is 16000 * 1 * 2 * 1, or 16000 * 2. However, I could only get one recording and playback cycle at this size. It seemed like there was some buffer overrun issue. To overcome this problem, I added a slight pad to the buffer of 50%. Originally I doubled the size of the buffer, but this seemed like it would consume too much memory. I have not tried 10% or 25% to see if this still works.

The other issue was with access to the microphone and speaker. The controller can only record or playback. There are API calls that control the microphone and speaker e.g. M5.Mic.begin(),  M5.Mic.end(), M5.Mic.record(), M5.Mic.isEnabled() and M5.Mic.isRecording(). The speaker has a similar set of API calls e.g. M5.Speaker.begin(), M5.Speaker.playRaw() etc. 

When you do this:

	M5.Mic.begin();
	M5.Mic.record(… params ...);

The M5.Mic.record(), and M5.Speaker.playRaw(), API calls are asynchronous, so your sketch will continue to execute. You can check poll M5.Mic.isRecording() to see if the microphone is still recording sound. When the recording is done, you can use M5.Mic.end() to disable recording. After calling this API, I was under the impression that M5.Mic.isEnabled() would return false as I told the microphone to turn off. However, this is not the case and the API call always returns true. It might be a different story if I power off the device, but that’s going pretty far. 

I hoped to use the M5.device.isEnabled() API calls to determine the current state of the device, but was unable to do so. I overcame this issue by adding my own program state which is either waiting, recording or playing and having the various stages set the correct program state.

## Next Steps

1. The range of sounds effects could be extended. This was just a quick and fun project to see if I could make my voice sound odder than usual. Sound is recorded as a raw WAV file, which means it could be processed in other ways than just playback speed.

2. The more I look at the interface, the less intuitive it becomes. I might change the buttons around to give them better meaning e.g. + and - might be better used for the slower and faster playback speeds rather than < and >.
       
