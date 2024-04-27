/*
//  For reference this program utilizes the Common Gateway Interface
//  Hence why we define the content-type to html so that we can display 
//  information on the webserver.
*/
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    std::cout << "Content-type: text/html\r\n\r\n"; 
    std::cout << "<html><body>";
    std::cout << "<h1>Login Debugged</h1>";

    int flag = 0;
    char u_Input[8], p_Input[10]; // User credential input

    if (getenv("REQUEST_METHOD") && strcmp(getenv("REQUEST_METHOD"), "POST") == 0) // Checks if the form has been submitted via POST method
    {
        int form_Length = atoi(getenv("CONTENT_LENGTH"));  // Converts CONTENT_LENGTH string to int
        char *form_Data = (char *)malloc(form_Length + 1); // Creates a char array from the form length gathered above
        fread(form_Data, 1, form_Length, stdin);           // Reads data from stdin for the size of form_length to form_data
        form_Data[form_Length] = '\0';                     // Adds null terminater to array

        sscanf(form_Data, "username=%[^&]&password=%s", u_Input, p_Input); // reads data from form_Data and seperates it for username and password
        free(form_Data);                                                   

        char username[8] = "admin"; // The corrct username
        char password[10] = "password"; // The correct password
        
        //For Debugging Purposes and Presentation
        std::cout << "<p>uInput: " << u_Input << "</p>"; // Prints username input
        std::cout << "<p>pInput: " << p_Input << "</p>"; // Prints password input
        std::cout << "<p>username: " << username << "</p>"; // Prints the correct username
        std::cout << "<p>password: " << password << "</p>"; // Prints the correct password

        /////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////      Buffer overflow Exploitation Explanation        /////////////////////////////
        // Since our array p_Input is set to hold 9 elements 0-9 and 1 for the null terminator we can  //
        // enter in 10 elements which would take the spot of the null terminator which would allow the //
        // array to be overflowed. Then data from other items in the stack that were entered in after  //
        // the p_input item was, would have there data read into the p_input array.                    //
        // Example: user inputs 'applesauce' in the passwrod field, when the form is submitted the     //  
        // p_input array will equal 'applesaucepassword'. This allows us to remove the users input     //   
        // from the array by shifting the elements in the array foward by 10 slots. Which makes the    //
        // p_input array now eqaul to password which grants us access to the system because once       //
        // the credentials are checked p_input equals password.                                        //
        /////////////////////////////////////////////////////////////////////////////////////////////////
        if (strlen(p_Input) > 9) 
        {
            char *memory = (char *)malloc(1000000000);
            for (long i = 0; i < 1000000000; i++) //Used to create a memory spike(Indicated something malicious is happening) For demo purposes
            {
                memory[i] = 'A';
            }

            for (int i = 0; i < 10; i++) // Modifies p_input to remove the users input
            {
                p_Input[i] = p_Input[i + 10];
            }
            std::cout << "<p>Modified Input: " << p_Input << "</p>"; // Prints out the modified input for demo purposes
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////

        if (std::strncmp(username, u_Input, 8) == 0 && std::strncmp(password, p_Input, 8) == 0) //Checks if the entered credentials are corret or not
        {
            std::cout << "<h3>Access Granted</h3>";
        }else{
            std::cout << "<h3>Access Denied</h3>";
        }
    }
    std::cout << "</body></html>";
    return 0;
}
