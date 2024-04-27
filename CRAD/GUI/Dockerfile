# Use an official Qt image as the base image
FROM qt:5

# Set the working directory in the container
WORKDIR /app

# Copy the source code and resources into the container
COPY . /app

# Install any needed dependencies
# (Note: You may need to adjust this depending on your application's dependencies)
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Compile the C++ application
RUN qmake && make

# Set display environment variable (you may need to adjust this depending on your setup)
ENV DISPLAY=:0

# Run the application
CMD ["./ContainerMetricsFINAL"]
