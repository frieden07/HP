#include "bits/stdc++.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
#include "rapidjson/include/rapidjson/rapidjson.h"
#include "rapidjson/include/rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

class JSONFeatures {
public:
    static string DeserializeFromFile(const string& filePath) {
        ifstream f(filePath);
        stringstream buffer;
        buffer << f.rdbuf();
        f.close();
        return buffer.str();
    }

    static void SerializeToFile(const string& filePath, string& s) {
        ofstream f(filePath);
        f << s;
        f.flush();
        f.close();
    }

    static bool InitDocument(const std::string& s, Document& doc) {
        if (s.empty())
            return false;
        string validJson(s);
        return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;
    }
};

class Student {

private:
    string _name, _class;
    float _score;
    int _id;

public:
    Student() {}
    Student(int id, string name, string clas, float score) { Id(id); Name(name); CLass(clas); Score(score); }

    void Deserialize(const rapidjson::Value& obj) {
        Id(obj["id"].GetInt());
        Name(obj["name"].GetString());
        CLass(obj["class"].GetString());
        Score(obj["score"].GetFloat());
    }

    template <typename Writer>
    void Serialize(Writer& writer) const {
        writer.StartObject();

        writer.String("id");
        writer.Int(_id);

        writer.String("name");
        writer.String(_name.c_str());

        writer.String("class");
        writer.String(_class.c_str());

        writer.String("score");
        writer.Double(_score);

        writer.EndObject();
    }

    const string& Name() {
        return _name;
    }
    void Name(const string& name) {
        _name = name;
    }

    const string& CLass() {
        return _class;
    }
    void CLass(const string& clas) {
        _class = clas;
    }

    float Score() {
        return _score;
    }
    void Score(float score) {
        _score = score;
    }

    int Id() {
        return _id;
    }

    void Id(int id) {
        _id = id;
    }
};


int main(int, char* []) {
    string filepath = "Jsonfile.json";
    vector< Student > students;
    int numberOfEntries = 0;

    int flag = 1;
    cout << "\n\t\t\t --------- Serialize and Deserialize ------------- ";

    while (flag == 1) {
        cout << "\n\nUser Manual :\n1. To Deserialize (Read from json - convert Json to string) \n2. To Seialize (write into json - Convert string to Json)\n3. To exit";
        cout << "\n\nEnter choice : ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string buffer = JSONFeatures::DeserializeFromFile(filepath);
            Document parsedBuffer;
            if (JSONFeatures::InitDocument(buffer, parsedBuffer) == true) {
                for (rapidjson::Value::ConstValueIterator itr = parsedBuffer.Begin(); itr != parsedBuffer.End(); ++itr) {
                    Student p;
                    p.Deserialize(*itr);
                    students.push_back(p);
                    numberOfEntries++;
                }
            }
            StringBuffer sb;
            PrettyWriter <StringBuffer> writer(sb);


            writer.StartArray();
            for (int i = 0; i < numberOfEntries; i++)
                students[i].Serialize(writer);
            writer.EndArray();

            string s = sb.GetString();
            cout << "JSON file data:\n";
            cout << s << "\n";
        }
        else if (choice == 2) {
            int id;
            string name, cl;
            float score;
            cout << "Enter Details :\nId : ";
            cin >> id;
            cout << "Name : ";
            cin >> name;
            cout << "Class : ";
            cin >> cl;
            cout << "Score : ";
            cin >> score;
            Student newStudent(id, name, cl, score);
            students.push_back(newStudent); numberOfEntries++;

            StringBuffer nsb;
            PrettyWriter <StringBuffer> nwriter(nsb);
            nwriter.StartArray();
            for (int i = 0; i < numberOfEntries; i++)
                students[i].Serialize(nwriter);
            nwriter.EndArray();

            string s = nsb.GetString();
            JSONFeatures::SerializeToFile(filepath, s);
        }
        else if (choice == 3) {
            cout << "\nSuccessfully Terminated Program!!";
            flag = 0;
        }
        else {
            cout << "\nInvalid Choice!!";
        }
    }

    return 0;
}