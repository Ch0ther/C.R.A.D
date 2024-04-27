# The apache webserver is used to run the validate.cpp application on it

# Step 1) This web server is setup by using the ubuntu docker image. 
#   The reason we did it this way rather than using the httpd apache image 
#   is because this was the easiest way to enable apache CGI and we could not 
#   find a way to enable the CGI module with the httpd image.
FROM ubuntu:latest 

# Step 2) We then installed apache on it.
RUN apt-get update && \
    apt-get install -y apache2 && \
    rm -rf /var/lib/apt/lists/*

# Step 3) Then we Enabled the CGI module. 
RUN a2enmod cgi

# Step 4) Exposed the port we wanted
EXPOSE 80

# Step 5) Lastly we started the apache server in the foreground
CMD ["apache2ctl", "-D", "FOREGROUND"]
