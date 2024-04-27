import docker
import json
import time
def detect_anomalies(memory, cpu): # Compares anomalies against baseline
                                     
    if memory > 50.0: 
        print("CPU Usage:","%.3f" % cpu , "%")
        print("Memory Usage:","%.3f" % memory, "MiB\n")              
        return 1                                                    
    else:
        print("No anomalies detected")
        print("CPU Usage:","%.3f" % cpu , "%")
        print("Memory Usage:","%.3f" % memory, "MiB\n")     

    return 0
    
def main():

    client = docker.from_env() # Gets docker client 

    errFlag = 0  # will be used to count the num of iterations for the overflow and restart when hitting a certain threshold 
    #api = docker.APIClient(base_url='unix://var/run/docker.sock') # Grabs the api_client for inspection 
    webServerContainer = client.containers.get("c_r_a_d-cpp_webserver-1")
    thisContainer = client.containers.get("detection_Container")
    #htmlContainer = client.containers.get("c_r_a_d-html_webserver-1")
    #appContainer = client.containers.get("c_r_a_d-application-1")

    

    print("Webserver Container ID:", webServerContainer.short_id)                                     
    
    containerStats = webServerContainer.stats(stream=True) # Streams container stats like 'docker stats' command

    for statsBytes in containerStats:
        stats = json.loads(statsBytes.decode('utf-8')) # Converts byte string to regular string

        if 'memory_stats' in stats and 'usage' in stats['memory_stats']: # Ensures it exits
            memUsage = stats['memory_stats']['usage'] # Gets container's memory usage
            memInMebi =  memUsage / (1048576) # Calculates recieved memusage to mebibytes
        else:
            memInMebi = 0

        cpuDelta = stats['cpu_stats']['cpu_usage']['total_usage'] - stats['precpu_stats']['cpu_usage']['total_usage'] # Gets the cpu delta of the container(this is by taking curr cpu by pre cpu)

        if 'system_cpu_usage' in stats['precpu_stats']: # Error handling. This doesn't work when the program begins because precpu obviously doesn't exist. Therefore only run this if it does 
            systemDelta = stats['cpu_stats']['system_cpu_usage'] - stats['precpu_stats']['system_cpu_usage'] # Same as cpu delta except system based 
            cpuPerc = (cpuDelta / systemDelta) * len(stats['cpu_stats']['cpu_usage']['percpu_usage']) * 100 # Calculate the cpu percentage as docker stats does (len gets amount of cores)
        else:
                   
            cpuPerc  = 0 # Default to 0 cuz pre cpu doesn't exist

        if cpuPerc > 0.20: # Waits until cpu percent threshold is met. Indicates form submission
             errFlag += detect_anomalies(memInMebi, cpuPerc)
             if errFlag == 1: # anomalies detected for 1 iterations
                print("Buffer-Overflow occurring!")
                print("Webserver restarting...")
                errFlag = 0
                webServerContainer.restart()
                webServerContainer.exec_run("apache2ctl restart")
                thisContainer.restart()


                

          
        else:                               
            print("CPU Usage:","%.3f" % cpuPerc , "%")
            print("Memory Usage:","%.3f" % memInMebi, "MiB\n") 
                 
   
if __name__ == "__main__":                         
    main()