#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cmath>
#include <cstring>

struct pixel {
    int red, green, blue;
};

struct header {
    int format = -1;
    uint64_t horizontal_size = 0;
    uint64_t vertical_size = 0;
    uint64_t max_num = 0;
};

bool inline_format = false;
header header;

void skip(std::vector<unsigned char> image, unsigned long &i) {
    while (image[i] <= 47 || image[i] >= 58) {
        if (image[i] == 35) {
            while (i < image.size() && image[i] != 10) {
                i++;
            }
            i++;
        } else {
            i++;
        }
    }
}

uint64_t get_number(std::vector<unsigned char> image, unsigned long &i) {
    uint64_t result = 0;
    while (image[i] > 47 && image[i] < 58) {
        result *= 10;
        result += (image[i] - 48);
        i++;
    }
    return result;
}

void getFormat(std::vector<unsigned char> image) {
    if (+static_cast<unsigned char>(image[0]) == 80) {
        header.format = -48 + (+static_cast<unsigned char>(image[1]));
    }
    if (header.format < 1 || header.format > 6) {
        std::cout << "invalid format" << std::endl;
        exit(1);
    }
}

unsigned long getSize(std::vector<unsigned char> image) {
    if (image[2] == 32) {
        inline_format = true;
    }
    unsigned long i = 2;
    while (i < image.size()) {
        if (image[i] == 10 || (inline_format && image[i] == 32)) {
            if (image[++i] != 35) {
                header.horizontal_size = get_number(image, i);
                i++;
                header.vertical_size = get_number(image, i);
                if (!(header.format == 1 || header.format == 4)) {
                    while (!(image[i] > 47 && image[i] < 58)) {
                        i++;
                    }
                    header.max_num = get_number(image, i);
                }
                break;
            }
        }
        i++;
    }
    if (inline_format) {
        if (image[i] == 32) {
            i++;
        }
    } else {
        while (!(image[i] == 10 && image[i + 1] != 35)) {
            i++;
        }
        i++;
    }
    if (header.horizontal_size < 1 || header.vertical_size < 1) {
        std::cout << "wrong dimensions\n";
        exit(1);
    }
    return i;
}

std::vector<std::vector<pixel>> P1_to_universal(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    universal_data.resize(header.vertical_size);
    unsigned long width = 0;
    unsigned long height = 0;
    unsigned long i = data_start_index;
    while (height != header.vertical_size) {
        skip(image, i);
        if (image[i] == 48 || image[i] == 49) {
            pixel new_pixel;
            new_pixel.red = new_pixel.green = new_pixel.blue = (image[i] - 49) * 255 * -1;
            universal_data[height].push_back(new_pixel);
            width++;
            i++;
            if (width == header.horizontal_size) {
                width = 0;
                height++;
            }
        }
    }
    return universal_data;
}

std::vector<std::vector<pixel>> P2_to_universal(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    universal_data.resize(header.vertical_size);
    double multiplier = 255 / header.max_num;
    unsigned long width = 0;
    unsigned long height = 0;
    unsigned long i = data_start_index;
    while (height != header.vertical_size) {
        skip(image, i);
        uint64_t number = get_number(image, i);
        pixel new_pixel;
        new_pixel.red = new_pixel.green = new_pixel.blue = std::min(static_cast<int>(round(number * multiplier)), 255);

        universal_data[height].push_back(new_pixel);
        width++;
        if (width == header.horizontal_size) {
            width = 0;
            height++;
        }
    }
    return universal_data;
}

std::vector<std::vector<pixel>> P3_to_universal(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    universal_data.resize(header.vertical_size);
    unsigned long width = 0;
    unsigned long height = 0;
    unsigned long i = data_start_index;
    while (height != header.vertical_size) {
        pixel new_pixel;
        skip(image, i);
        new_pixel.red = get_number(image, i) * (255 / header.max_num);
        skip(image, i);
        new_pixel.green = get_number(image, i) * (255 / header.max_num);
        skip(image, i);
        new_pixel.blue = get_number(image, i) * (255 / header.max_num);

        universal_data[height].push_back(new_pixel);
        width++;
        if (width == header.horizontal_size) {
            width = 0;
            height++;
        }
    }
    return universal_data;
}

std::vector<std::vector<pixel>> P4_to_universal(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    universal_data.resize(header.vertical_size);
    unsigned long width = 0;
    unsigned long height = 0;
    unsigned long i = data_start_index;
    while (height != header.vertical_size) {
        unsigned char value = image[i];
        bool bits[8];
        for (int j = 7; j > -1; j--, value >>= 1) {
            bits[j] = value & 1;
        }
        unsigned long iterator = std::min(header.horizontal_size - width, static_cast<unsigned long long>(8));
        for (unsigned long j = 0; j < iterator; j++) {
            pixel new_pixel;
            if (bits[j]) {
                new_pixel.red = new_pixel.green = new_pixel.blue = 0;
            } else {
                new_pixel.red = new_pixel.green = new_pixel.blue = 255;
            }
            universal_data[height].push_back(new_pixel);
            width++;
            if (width >= header.horizontal_size) {
                width = 0;
                height++;
            }
        }
        i++;
    }
    return universal_data;
}

std::vector<std::vector<pixel>> P5_to_universal(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    universal_data.resize(header.vertical_size);
    unsigned long width = 0;
    unsigned long height = 0;
    unsigned long i = data_start_index;
    while (height != header.vertical_size) {
        pixel new_pixel;
        new_pixel.red = new_pixel.green = new_pixel.blue = std::min((int) image[i], 255);
        universal_data[height].push_back(new_pixel);
        width++;
        i++;
        if (width == header.horizontal_size) {
            width = 0;
            height++;
        }
    }
    return universal_data;
}

std::vector<std::vector<pixel>> P6_to_universal(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    universal_data.resize(header.vertical_size);
    unsigned long width = 0;
    unsigned long height = 0;
    unsigned long i = data_start_index;
    while (height != header.vertical_size) {
        pixel new_pixel;
        new_pixel.red = (int) image[i];
        i++;
        new_pixel.green = (int) image[i];
        i++;
        new_pixel.blue = (int) image[i];
        i++;
        universal_data[height].push_back(new_pixel);
        width++;
        if (width == header.horizontal_size) {
            width = 0;
            height++;
        }
    }
    return universal_data;
}

std::vector<std::vector<pixel>>
universal_data_parser(std::vector<unsigned char> image, unsigned long data_start_index) {
    std::vector<std::vector<pixel>> universal_data;
    if (header.format == 1) {
        universal_data = P1_to_universal(std::move(image), data_start_index);
    } else if (header.format == 2) {
        universal_data = P2_to_universal(std::move(image), data_start_index);
    } else if (header.format == 3) {
        universal_data = P3_to_universal(std::move(image), data_start_index);
    } else if (header.format == 4) {
        universal_data = P4_to_universal(std::move(image), data_start_index);
    } else if (header.format == 5) {
        universal_data = P5_to_universal(std::move(image), data_start_index);
    } else if (header.format == 6) {
        universal_data = P6_to_universal(std::move(image), data_start_index);
    }
    return universal_data;
}

void convert_image(char *format, std::vector<std::vector<pixel>> &universal_image) {
    if ((strcmp(format, "P1") == 0) || (strcmp(format, "P4") == 0)) {
        for (auto &row: universal_image) {
            for (auto &pixel: row) {
                double mean = round(static_cast<double>(pixel.red + pixel.green + pixel.blue) / 3);
                if (mean > 127) {
                    pixel.red = pixel.blue = pixel.green = 0;
                } else {
                    pixel.red = pixel.blue = pixel.green = 1;
                }
            }
        }
    } else if ((strcmp(format, "P2") == 0) || (strcmp(format, "P5") == 0)) {
        for (auto &row: universal_image) {
            for (auto &pixel: row) {
                int mean = static_cast<int>(round(static_cast<double>(pixel.red + pixel.green + pixel.blue) / 3));
                pixel.red = pixel.blue = pixel.green = mean;
            }
        }
    }
}

void write(char *format, std::vector<std::vector<pixel>> universal_image, std::ofstream &output_file) {
    output_file << format << "\n";
    output_file << header.horizontal_size << " " << header.vertical_size << "\n";
    if (!((strcmp(format, "P1") == 0) || (strcmp(format, "P4") == 0))) {
        output_file << 255 << "\n";
    }
    if (strcmp(format, "P1") == 0 || strcmp(format, "P2") == 0) {
        for (int i = 0; i < header.vertical_size; i++) {
            for (int j = 0; j < header.horizontal_size; j++) {
                output_file << universal_image[i][j].red << " ";
            }
            output_file << "\n";
        }
    }
    if (strcmp(format, "P3") == 0) {
        for (int i = 0; i < header.vertical_size; i++) {
            for (int j = 0; j < header.horizontal_size; j++) {
                output_file << universal_image[i][j].red
                            << " " << universal_image[i][j].green
                            << " " << universal_image[i][j].blue << " ";
            }
            output_file << "\n";
        }
    }
    if (strcmp(format, "P4") == 0) {
        int height, width;
        height = width = 0;
        while (height != header.vertical_size) {
            for (unsigned long row_repeat = 1; row_repeat <= ((header.horizontal_size-1)/8)+1; row_repeat++){
                int byte_value = 0;
                int multiplier = 128;
                int bits_left = header.horizontal_size-width;
                for (unsigned long one_byte = 0; one_byte < std::min(8,bits_left); one_byte++){
                    byte_value += universal_image[height][width].red * multiplier;
                    multiplier /= 2;
                    width++;
                }
                output_file << static_cast<char>(byte_value);
            }
            width = 0;
            height++;
        }
    }
    if (strcmp(format, "P5") == 0) {
        for (int i = 0; i < header.vertical_size; i++) {
            for (int j = 0; j < header.horizontal_size; j++) {
                output_file << static_cast<char>(universal_image[i][j].red);
            }
        }
    }
    if (strcmp(format, "P6") == 0) {
        for (int i = 0; i < header.vertical_size; i++) {
            for (int j = 0; j < header.horizontal_size; j++) {
                output_file << static_cast<char>(universal_image[i][j].red)
                            << static_cast<char>(universal_image[i][j].green)
                            << static_cast<char>(universal_image[i][j].blue);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "invalid no of arguments";
        exit(1);
    }
    std::ifstream input_file(argv[1], std::ios::binary | std::ios::in);
    std::ofstream output_file(argv[2]);
    if (!input_file.is_open() || !output_file.is_open()) {
        std::cout << "cannot read or write file\n";
        return 1;
    }
    if (!(strcmp(argv[3],"P1") == 0 || strcmp(argv[3],"P2") == 0 || strcmp(argv[3],"P3") == 0||
        strcmp(argv[3],"P4") == 0 || strcmp(argv[3],"P5") == 0 || strcmp(argv[3],"P6") == 0)){
        std::cout << "invalid format to convert" << std::endl;
        exit(1);
    }

    char buffer[2048];
    std::vector<unsigned char> image;

//    READ FILE AND APPEND ARRAY
    while (input_file.good()) {
        input_file.read(buffer, 2047);
        buffer[input_file.gcount()] = '\0';
        for (int i = 0; i < input_file.gcount(); i++) {
            image.push_back(static_cast<unsigned char>(buffer[i]));
        }
    }

//    PARSE HEADER FROM ARRAY
    getFormat(image);
    unsigned long data_start_index = getSize(image);
    auto universal_data = universal_data_parser(image, data_start_index);
    convert_image(argv[3], universal_data);
    write(argv[3], universal_data, output_file);
}