#include <iostream>
#include <rapp-robots-api/communication/communication.hpp>
#include <rapp-robots-api/info/info.hpp>
#include <map>
#include <rapp-cloud-api/send_email.hpp>
#include "Contact_list.hpp"
#include <rapp/objects/picture/picture.hpp>
#include <chrono>
#include <thread>
#include <rapp-robots-api/vision/vision.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#define host "roman"


int main(int argc, char * argv[]) {
	for (int i = 1; i < argc; ++i) {
		std::cout << argv[i] << "\n";
	}
    rapp::robot::info info(argc, argv);
	rapp::robot::vision vis(argc, argv);
    rapp::robot::communication comm(argc, argv);
		
    std::string type = "personal";
    
	if(type == "personal") {
		comm.text_to_speech("Do you want to send email?");

		// create list of words for recognition
		std::vector<std::string> words;
		// add two words
		words.push_back("yes");
		words.push_back("no");
		std::string result;
		std::string name;
		
		Contact_list contact_list;
		
		contact_list.add_contact("anna","aniawujek@gmail.com");
		contact_list.add_contact("bartek","bartswis@gmail.com");
		contact_list.add_contact("vega","wegierek.maciej@gmail.com");
		
		rapp::object::picture::Ptr picture;
		
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
			
			
			
			
			/// docelowo ten fragment powinien byc zrealizowany za pomoca rozpoznawania mowy
			//comm.text_to_speech("Please type the message and press enter");
			//std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
			//std::getline(std::cin, message);
			/// docelowo ten fragment powinien byc zrealizowany za pomoca rozpoznawania mowy
			
			message = "pozdrowienia od Nao";
			
			message = "Dear " + name + "!\n\n" + message + "\n\n XOXO\nZbyszek Zbyniu Zbyszkowski";
			comm.text_to_speech("Sending message: " + message);	
			std::vector<std::string> attachments = {};
			
			do {
				comm.text_to_speech("Do you want to attach a photo?");	
				result = comm.word_spotting(words);

				if (result == "no") {
					comm.text_to_speech("ok");
				}
				else if (result == "yes") {
					
					comm.text_to_speech("Show me something!");
					std::this_thread::sleep_for(std::chrono::seconds(2));
					picture = vis.capture_image(0, 3, "png");
					std::string filepath = "/tmp/selfie.png";
					picture->save(filepath);
					attachments.push_back(filepath); 
					
					/*** robienie zdjecia i zapisanie do pliku i do attachments ***/
					
					message = message + "\n\nPS Please see attached photo.";
				}
			} while (result != "yes" && result != "no");	
			
			//send email with message
			rapp::cloud::send_email (
				"zbyszekzbyniuzbyszkowski@gmail.com", // user email
				"zbyszekzbyniu", // password
				"smtp.gmail.com", // server
				"587", // port
				{contact_list.get_email_address(name)},
				"Subject", // subject
				message, // body
				attachments, // list of attachements
				host
			);
				
			comm.text_to_speech("Message sent to " + name + ". Do you want to send next email?");
			do {
				result = comm.word_spotting(words);

				if (result == "no") {
					comm.text_to_speech("bye");
					//return 0;
				}

				else if (result != "yes") {
					comm.text_to_speech("Say yes to send email or no to finish");
				}
			} while (result != "yes" && result != "no");
			
		} while(result == "yes");
		comm.text_to_speech("Bye!");
	}
	
	type = "report";
	if(type == "report") {
		
		Contact_list contact_list;
				
		contact_list.add_contact("Anna","aniawujek@gmail.com");
		contact_list.add_contact("Bartek","bartswis@gmail.com");
		contact_list.add_contact("Vega","wegierek.maciej@gmail.com");		
		
		std::string rep_message;
		
		std::ofstream f1;
		f1.open("/tmp/report.txt");
		f1 << "raport raport raport";
		f1.close();

		
		std::string rep = "/tmp/report.txt";
		
		std::vector<std::string> reports = {rep};
		
		
		rep_message = "Dear son of Zbyszek Zbyszkowski!\n\nPlease find attached my daily report.\n\nXOXO\nSuper Robot";
			
		
		//send email with message
		rapp::cloud::send_email (
			"zbyszekzbyniuzbyszkowski@gmail.com", // user email
			"zbyszekzbyniu", // password
			"smtp.gmail.com", // server
			"587", // port
			{contact_list.get_email_address("Anna")},
			"Daily report", // subject
			rep_message, // body
			reports, // list of attachements
			host
		);
		comm.text_to_speech("I just sent your daily report");	

	}
    
    
    return 0;
}
