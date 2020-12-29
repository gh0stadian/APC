#include <queue>
#include <iostream>
#include <fstream>

struct byte_count
{
    int byte;
    uint64_t count;
    byte_count *left, *right;

    bool operator< (const byte_count & p)const {
        if(count==p.count) {
            return byte>p.byte;
        }
        else {
            return count>p.count;
        }
    }
};

std::priority_queue<byte_count> init_queue(uint64_t *byteCount) {
    std::priority_queue<byte_count> pq;
    for (int i = 0; i < 256; i++) {
        if (byteCount[i] == 0) {
            continue;
        } else {
            byte_count node = {i, byteCount[i], nullptr, nullptr};
            pq.push(node);
        }
    }
    return pq;
}

byte_count constructTree(std::priority_queue<byte_count> pq){
    byte_count *bc1, *bc2;
    while (pq.size() != 1){
        bc1 = new byte_count;
        bc2 = new byte_count;
        *bc1 = pq.top();
        pq.pop();
        *bc2 = pq.top();
        pq.pop();
        byte_count merged_node{};
        merged_node = {256,bc1->count+bc2->count, bc1, bc2};
        pq.push(merged_node);
    }
    return pq.top();
}

void fill_result_array(struct byte_count* node, std::string value, std::string *result){
    if (node == nullptr){
        return;
    }
    fill_result_array(node->left, value + '0', result);
    if (node->byte != 256){
        result[node->byte]=value;
    }
    fill_result_array(node->right, value + '1', result);
}

void deleteTree(struct byte_count* node, struct byte_count* root){
    if (node->left != nullptr){
        deleteTree(node->left, root);
    }
    if (node->right != nullptr){
        deleteTree(node->right, root);
    }
    if (node != root){
        delete node;
    }
}

int main(int argc, char * argv[]){
    if(argc != 2){
        std::cout << "wrong parameters" << std::endl;
        return 1;
    }
//    std::ifstream input_file;
    std::ifstream input_file(argv[1], std::ios::binary);
    if(!input_file.is_open()){
        std::cout << "cannot read file\n";
        return 1;
    }

    uint64_t byteCount[256] = { 0 };
    char buffer[1024+1];
    // FILL ARRAY byteCount

    while (input_file.good()){
        input_file.read(buffer, 1024);
        buffer[input_file.gcount()]='\0';
        for(int i = 0; i < input_file.gcount(); i++){
            int index = +static_cast<unsigned char>(buffer[i]);
            byteCount[index]++;
        }
    }
    //    GET QUEUE FROM ARRAY
    std::priority_queue<byte_count> pq = init_queue(byteCount);
    if (pq.empty()){
        return 0;
    }
    byte_count root = constructTree(pq);
    std::string result[256] = {""};
    fill_result_array(&root, "", result);

    for (int i = 0; i < 256; i++) {
        if (result[i].empty()) {
            continue;
        } else {
            std::cout << i << ": " << result[i] << std::endl;
        }
    }

    deleteTree(&root, &root);
    return 0;
}