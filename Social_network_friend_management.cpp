
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

class User {
public:
    int id;
    string name;
    vector<int> friends;
    vector<int> requests;

    User(int userId, string userName) {
        id = userId;
        name = userName;
    }
};

class SocialNetwork {
private:
    vector<User> users;

    int findUserIndex(int id) const {
        for (int i = 0; i < (int)users.size(); i++) {
            if (users[i].id == id)
                return i;
        }
        return -1;
    }

    bool areFriends(int id1, int id2) const {
        int index = findUserIndex(id1);
        if (index == -1) return false;

        const auto& list = users[index].friends;
        return find(list.begin(), list.end(), id2) != list.end();
    }

    bool hasRequest(int senderId, int receiverId) const {
        int index = findUserIndex(receiverId);
        if (index == -1) return false;

        const auto& list = users[index].requests;
        return find(list.begin(), list.end(), senderId) != list.end();
    }

public:
    void addUser() {
        int id;
        string name;

        cout << "Enter unique user ID: ";
        cin >> id;

        if (findUserIndex(id) != -1) {
            cout << "This ID already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter user name: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Name cannot be empty!\n";
            return;
        }

        users.emplace_back(id, name);
        cout << "User added successfully!\n";
    }

    void displayUsers() const {
        if (users.empty()) {
            cout << "No users registered yet.\n";
            return;
        }

        cout << "\n--- ALL USERS ---\n";
        for (const auto& user : users) {
            cout << "ID: " << user.id
                 << " | Name: " << user.name
                 << " | Friends: " << user.friends.size()
                 << '\n';
        }
    }

    void sendFriendRequest() {
        int senderId, receiverId;

        cout << "Enter your user ID: ";
        cin >> senderId;
        cout << "Enter receiver user ID: ";
        cin >> receiverId;

        int sender = findUserIndex(senderId);
        int receiver = findUserIndex(receiverId);

        if (sender == -1 || receiver == -1) {
            cout << "User not found!\n";
            return;
        }

        if (senderId == receiverId) {
            cout << "You cannot send a request to yourself.\n";
            return;
        }

        if (areFriends(senderId, receiverId)) {
            cout << "You are already friends.\n";
            return;
        }

        if (hasRequest(senderId, receiverId)) {
            cout << "Request has already been sent.\n";
            return;
        }

        if (hasRequest(receiverId, senderId)) {
            cout << "This user has already requested you. Accept their request instead.\n";
            return;
        }

        users[receiver].requests.push_back(senderId);
        cout << "Friend request sent successfully!\n";
    }

    void viewRequests() const {
        int id;
        cout << "Enter your user ID: ";
        cin >> id;

        int index = findUserIndex(id);
        if (index == -1) {
            cout << "User not found!\n";
            return;
        }

        cout << "\n--- PENDING FRIEND REQUESTS ---\n";

        if (users[index].requests.empty()) {
            cout << "No pending requests.\n";
            return;
        }

        for (int requesterId : users[index].requests) {
            int requester = findUserIndex(requesterId);
            if (requester != -1) {
                cout << "ID: " << users[requester].id
                     << " | Name: " << users[requester].name << '\n';
            }
        }
    }

    void acceptFriendRequest() {
        int receiverId, senderId;

        cout << "Enter your user ID: ";
        cin >> receiverId;
        cout << "Enter request sender ID: ";
        cin >> senderId;

        int receiver = findUserIndex(receiverId);
        int sender = findUserIndex(senderId);

        if (receiver == -1 || sender == -1) {
            cout << "User not found!\n";
            return;
        }

        auto& requests = users[receiver].requests;
        auto it = find(requests.begin(), requests.end(), senderId);

        if (it == requests.end()) {
            cout << "No pending request from this user.\n";
            return;
        }

        requests.erase(it);
        users[receiver].friends.push_back(senderId);
        users[sender].friends.push_back(receiverId);

        cout << "Friend request accepted! You are now friends.\n";
    }

    void removeFriend() {
        int id1, id2;

        cout << "Enter your user ID: ";
        cin >> id1;
        cout << "Enter friend's user ID: ";
        cin >> id2;

        int index1 = findUserIndex(id1);
        int index2 = findUserIndex(id2);

        if (index1 == -1 || index2 == -1) {
            cout << "User not found!\n";
            return;
        }

        if (!areFriends(id1, id2)) {
            cout << "These users are not friends.\n";
            return;
        }

        auto& list1 = users[index1].friends;
        list1.erase(remove(list1.begin(), list1.end(), id2),
                    list1.end());

        auto& list2 = users[index2].friends;
        list2.erase(remove(list2.begin(), list2.end(), id1),
                    list2.end());

        cout << "Friend removed successfully.\n";
    }

    void displayFriends() const {
        int id;
        cout << "Enter user ID: ";
        cin >> id;

        int index = findUserIndex(id);
        if (index == -1) {
            cout << "User not found!\n";
            return;
        }

        cout << "\nFriends of " << users[index].name << ":\n";

        if (users[index].friends.empty()) {
            cout << "No friends yet.\n";
            return;
        }

        for (int friendId : users[index].friends) {
            int friendIndex = findUserIndex(friendId);
            if (friendIndex != -1) {
                cout << "ID: " << users[friendIndex].id
                     << " | Name: " << users[friendIndex].name << '\n';
            }
        }
    }

    void mutualFriends() const {
        int id1, id2;

        cout << "Enter first user ID: ";
        cin >> id1;
        cout << "Enter second user ID: ";
        cin >> id2;

        int index1 = findUserIndex(id1);
        int index2 = findUserIndex(id2);

        if (index1 == -1 || index2 == -1) {
            cout << "User not found!\n";
            return;
        }

        cout << "\n--- MUTUAL FRIENDS ---\n";
        bool found = false;

        for (int friendId : users[index1].friends) {
            if (areFriends(id2, friendId)) {
                int friendIndex = findUserIndex(friendId);
                if (friendIndex != -1) {
                    cout << users[friendIndex].name
                         << " (ID: " << friendId << ")\n";
                    found = true;
                }
            }
        }

        if (!found)
            cout << "No mutual friends found.\n";
    }

    void searchUser() const {
        int id;
        cout << "Enter user ID to search: ";
        cin >> id;

        int index = findUserIndex(id);

        if (index == -1) {
            cout << "User not found.\n";
            return;
        }

        cout << "User found!\n";
        cout << "ID: " << users[index].id << '\n';
        cout << "Name: " << users[index].name << '\n';
        cout << "Friends: " << users[index].friends.size() << '\n';
        cout << "Pending requests: "
             << users[index].requests.size() << '\n';
    }
};

int main() {
    SocialNetwork network;
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " SOCIAL NETWORK FRIEND MANAGEMENT\n";
        cout << "====================================\n";
        cout << "1. Add User\n";
        cout << "2. Display All Users\n";
        cout << "3. Send Friend Request\n";
        cout << "4. View Friend Requests\n";
        cout << "5. Accept Friend Request\n";
        cout << "6. Remove Friend\n";
        cout << "7. View Friend List\n";
        cout << "8. Find Mutual Friends\n";
        cout << "9. Search User\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: network.addUser(); break;
            case 2: network.displayUsers(); break;
            case 3: network.sendFriendRequest(); break;
            case 4: network.viewRequests(); break;
            case 5: network.acceptFriendRequest(); break;
            case 6: network.removeFriend(); break;
            case 7: network.displayFriends(); break;
            case 8: network.mutualFriends(); break;
            case 9: network.searchUser(); break;
            case 0: cout << "Thank you for using the system!\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
