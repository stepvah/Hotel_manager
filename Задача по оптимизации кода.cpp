// Here is main function
#include "profile.h"
#include "test_runner.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>
#include <map>
#include <numeric>
#include <iomanip>
#include <queue>

using namespace std;



class Hotel {
public:
    void Book(long long time,
        int client_id, int room_count) {
        rooms += room_count;

        rooms_booking.push({ time, room_count });
        last_clients.insert(client_id);
        number_of_bookings[client_id]++;
        clients_booking.push({ time, client_id });
    }

    int Clients(long long time) {
        Adjust(time);

        return last_clients.size();
    }

    int Rooms(long long time) {
        Adjust(time);

        return rooms;
    }

private:
    static const int DAY_TIME = 86400;

    int rooms = 0;
    queue<pair<long long, int>> rooms_booking;
    set<int> last_clients;
    map<int, int> number_of_bookings;
    queue<pair<long long, int>> clients_booking;

    void Adjust(long long cur_time) {
        while (!rooms_booking.empty() &&
            rooms_booking.front().first <= cur_time - DAY_TIME) {
            rooms -= rooms_booking.front().second;
            rooms_booking.pop();

            number_of_bookings[clients_booking.front().second]--;
            if (number_of_bookings[clients_booking.front().second] == 0) {
                last_clients.erase(clients_booking.front().second);
            }
            clients_booking.pop();
        }
    }
};

class Booking {
private:
    map<string, Hotel> hotels;
    long long absolute_time = -1e18;
public:
    void Book(long long time, const string& hotel_name,
        int client_id, int room_count) {
        absolute_time = time;
        hotels[hotel_name].Book(time, client_id, room_count);
    }

    int Clients(const string& hotel_name) {
        if (hotels.count(hotel_name) == 0) {
            return 0;
        }
        return hotels.at(hotel_name).Clients(absolute_time);
    }

    int Rooms(const string& hotel_name) {
        if (hotels.count(hotel_name) == 0) {
            return 0;
        }
        return hotels.at(hotel_name).Rooms(absolute_time);
    }
};


void TestAll() {
    TestRunner tr;
    //RUN_TEST(tr, TestRead);
}

int main()
{
    TestAll();
    ios_base::sync_with_stdio(0);
    cin.tie(nullptr);
    LOG_DURATION("Total");
    
    Booking booking;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            long long time;
            cin >> time;
            string name;
            cin >> name;
            int client_id, room_count;
            cin >> client_id >> room_count;

            booking.Book(time, name, client_id, room_count);
        } 
        else if (query_type == "CLIENTS") {
            string name;
            cin >> name;

            cout << booking.Clients(name) << '\n';
        }
        else if (query_type == "ROOMS") {
            string name;
            cin >> name;

            cout << booking.Rooms(name) << '\n';
        }
    }
    return 0;
}