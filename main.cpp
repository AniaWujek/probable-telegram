#include <iostream>
#include <rapp-robots-api/communication/communication.hpp>
#include <rapp-robots-api/info/info.hpp>
#include <map>
#include <rapp-cloud-api/send_email.hpp>
#include "Contact_list.hpp"
#define host "roman"

int main(int argc, char * argv[]) {
    rapp::robot::info info(argc, argv);
    rapp::robot::communication comm(argc, argv);
    
    comm.text_to_speech("Do you want to send email?");
    
    // create list of words for recognition
    std::vector<std::string> words;
    // add two words
    words.push_back("yes");
    words.push_back("no");
    std::string result;
    std::string name;
    
    Contact_list contact_list;
    
    contact_list.add_contact("Anna","aniawujek@gmail.com");
    contact_list.add_contact("Bartek","bartswis@gmail.com");
    contact_list.add_contact("Vega","wegierek.maciej@gmail.com");
    
    
    do {
        result = comm.word_spotting(words);

        if (result == "no") {
            comm.text_to_speech("really?");
            return 0;
        }

        if (result != "yes") {
            comm.text_to_speech("Say yes to send email or no to finish");
        }
    } while (result != "yes");
    
    
    std::string message;
    do {		
		comm.text_to_speech("Please give the name");
		do {
			name = comm.word_spotting(contact_list.get_names());

			if(!contact_list.contact_exists(name)) {
				comm.text_to_speech("Please give the name");
			}
		} while (!contact_list.contact_exists(name));
		
		comm.text_to_speech("Please type the message and press enter");
		
		
		/// docelowo ten fragment powinien byc zrealizowany za pomoca rozpoznawania mowy
		std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, message);
		/// docelowo ten fragment powinien byc zrealizowany za pomoca rozpoznawania mowy
		
		message = "Dear " + name + "!\n\n" + message + "\n\n XOXO\nZbyszek Zbyniu Zbyszkowski";
		comm.text_to_speech("Sending message: " + message);		
		
		//send email with message
		rapp::cloud::send_email (
			"zbyszekzbyniuzbyszkowski@gmail.com", // user email
			"zbyszekzbyniu", // password
			"smtp.gmail.com", // server
			"587", // port
			{contact_list.get_email_address(name)},
			"Subject", // subject
			message, // body
			{}, // list of attachements
			host
		);
			
		comm.text_to_speech("Message sent to " + name + ". Do you want to send next email?");
		do {
			result = comm.word_spotting(words);

			if (result == "no") {
				comm.text_to_speech("bye");
				return 0;
			}

			if (result != "yes") {
				comm.text_to_speech("Say yes to send email or no to finish");
			}
		} while (result != "yes");
		
	} while(result == "yes");
    comm.text_to_speech("Bye!");
    std::cout << "Hello world! Application is run from: " << info.base_path() << "\n";
    return 0;
}