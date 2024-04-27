FROM python:3.9

WORKDIR /app

COPY MetricRetriever.py .
# Install docker
# Install docker
RUN apt-get update
RUN apt-get -y install apt-transport-https ca-certificates curl gnupg2 software-properties-common
RUN curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add -
RUN add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable"

RUN pip install docker

ENV PYTHONUNBUFFERED=1

CMD ["python", "MetricRetriever.py"]