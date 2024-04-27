# This Dockerfile is used to run our validate.cpp application, which 
#   then gets ran on the apache webserver via shared memory

# Step 1) Use the Ubuntu docker image.
FROM ubuntu:latest 

# Step 2) Install g++ to compile our validate.cpp application.
RUN apt-get update && \
    apt-get install -y g++

# Step 3) Copy validate.cpp into the directory.
COPY validate.cpp .

# Step 4) Compile validate.cpp
RUN g++ -o validate validate.cpp

# Step 5) Expose port 80.
EXPOSE 80

# Step 6) Run validate
CMD ["./validate"]

