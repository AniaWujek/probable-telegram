

class Contact_list {
	
	private:
		std::map<std::string, std::string> contact_list;
		std::vector<std::string> names;
	public:
		Contact_list(){};
		//true if contact exists
		bool contact_exists(std::string name) {
			return contact_list.find(name) != contact_list.end();
		}
		//Returns a bool denoting whether the insertion took place
		bool add_contact(std::string name, std::string email_address) {		
			contact_list.insert(std::pair<std::string, std::string>(name, email_address));
			/*if(result) {
				names.push_back(name);
			}*/
			return true;
		}
		//Returns email if name was found or empty string if not found
		std::string get_email_address(std::string name) {
			if(contact_exists(name)) {
				return contact_list.at(name);
			}
			else return "";
		}
		
		
		
		std::vector<std::string> get_names() {
			return names;
		}
		
};
