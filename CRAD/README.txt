README for C.R.A.D (Container-based Runtime Anomaly Detection)

To use C.R.A.D, please follow these steps after downloading all the files:

1) Download Docker Desktop 
2) Download and configure VScode with a Linux compiler (we used Ubuntu/WSL)
3) Download the Docker extension for VScode 
4) Download any Python compiler
5) In the terminal, run the following command: "pip install docker" *Note. Only follow this step if you intend to decontainerize the detection algorithm*
6) In the terminal, run the following command: "docker compose up"
7) Once containers are built, go into Docker Desktop 
8) Select "Containers" 
9) Look for a container named "1-html_webserver"
10) Select the port attached to it
11) Input information for the login screen 
    -correct username is admin, correct password is password
    -overflow the password field with any string longer than 10 characters
12) Go back to the terminal and watch the output of detection_Container
13) Repeat steps 10 and 11 for testing or press the previous page arrow on the browser tab 

IMPORTANT: If for some reason the HTML page does NOT take any more requests after an overflow, go into the VScode terminal and type "docker compose down", delete all images 
in Docker Desktop by selecting "Images", and then go back to the VScode terminal and type "docker compose up"

Credits: 
Brett Wortham (team leader), Nickolas Martin, Ryan Egloff