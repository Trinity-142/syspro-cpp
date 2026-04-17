#include <cassert>
#include <cstring>
#include <string>
#include <utility>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <iostream>

class IO {
protected:
    bool opened = false;
    bool eof = false;

public:
    IO(): opened(true) {}
    virtual ~IO() = default;
    virtual void close() = 0;
};

struct Reader: virtual IO {
    virtual size_t readBytes(void* buffer, size_t size) = 0;
    template<typename T>
    size_t read(T& value) {
        return readBytes(&value, sizeof(T));
    }

    size_t read(std::string& str, const size_t count) {
        if (!opened) return 0;
        str.resize(count);
        const size_t k = readBytes(&str[0], count);
        str.resize(k);
        return k;
    }
};

struct Writer: virtual IO {
    virtual size_t writeBytes(const void* buffer, size_t size) = 0;
    template<typename T>
    size_t write(const T& value) {
        return writeBytes(&value, sizeof(T));
    }

    size_t write(const std::string& str) {
        if (!opened) return 0;
        writeBytes(&str[0], str.size());
        return str.size();
    }
};

struct ReaderWriter: virtual Reader, virtual Writer {
    virtual void seek(size_t pos) = 0;
};

class StringReaderWriter: public virtual ReaderWriter {
    std::string data;
    size_t cursor = 0;

    size_t readBytes(void* buffer, const size_t size) override {
        if (!opened) return 0;
        if (cursor >= data.size()) {
            eof = true;
            return 0;
        }
        const size_t k = std::min(data.size() - cursor, size);
        if (k < size) {
            eof = true;
        }
        std::memcpy(buffer, data.data() + cursor, k);
        cursor += k;
        return k;
    }

    size_t writeBytes(const void* buffer, const size_t size) override {
        if (!opened) return 0;
        if (cursor + size > data.size()) {
            data.resize(cursor + size);
        }
        std::memcpy(data.data() + cursor, buffer, size);
        cursor += size;
        return size;
    }

public:
    StringReaderWriter(std::string data): data(std::move(data)) {}

    void close() override {
        if (opened) {
            opened = false;
        }
    }

    void seek(const size_t pos) override {
        if (!opened) return;
        cursor = std::min(pos, data.size());
        if (cursor < data.size()) {
            eof = false;
        }
    }
};

class FileReaderWriter: public virtual ReaderWriter {
    FILE* file;

    size_t readBytes(void* buffer, const size_t size) override {
        if (!opened) return 0;
        const size_t k = fread(buffer, 1, size, file);
        if (k < size) {
            if (feof(file)) {
                eof = true;
            } else if (ferror(file)) {
                fclose(file);
                opened = false;
            }
        }
        return k;
    }

    size_t writeBytes(const void* buffer, const size_t size) override {
        if (!opened) return 0;
        const size_t k = fwrite(buffer, 1, size, file);
        if (k < size) {
            if (ferror(file)) {
                fclose(file);
                opened = false;
            }
        }
        return k;
    }

public:
    FileReaderWriter(const char* filename): file(fopen(filename, "rb+")) {
        if (file == nullptr) {
            throw std::runtime_error("Cannot open file");
        }
    }

    ~FileReaderWriter() override {
        FileReaderWriter::close();
    }

    void close() override {
        if (opened) {
            fclose(file);
            opened = false;
        }
    }

    void seek(const size_t pos) override {
        if (!opened) return;
        if (fseek(file, static_cast<long>(pos), SEEK_SET) == 0) {
            eof = false;
        }
    }
};

template <size_t BufferSize = 1024>
class BufferedReaderWriter: public virtual ReaderWriter {
    ReaderWriter* source;

    std::vector<char> read_buf;
    size_t read_cursor = 0;
    std::vector<char> write_buf;

    void flush() {
        if (!write_buf.empty()) {
            source->writeBytes(write_buf.data(), write_buf.size());
            write_buf.clear();
        }
    }

    size_t readBytes(void* buffer, const size_t size) override {
        if (!opened) return 0;
        flush();

        size_t k = 0;
        while (k < size) {
            if (read_cursor >= read_buf.size()) {
                read_cursor = 0;
                read_buf.resize(BufferSize);
                const size_t actual = source->readBytes(read_buf.data(), BufferSize);
                read_buf.resize(actual);
                if (read_buf.empty()) {
                    eof = true;
                    break;
                }
            }

            const size_t to_copy = std::min(size - k, read_buf.size() - read_cursor);
            std::memcpy(static_cast<char*>(buffer) + k, read_buf.data() + read_cursor, to_copy);
            read_cursor += to_copy;
            k += to_copy;
        }
        return k;
    }

    size_t writeBytes(const void* buffer, const size_t size) override {
        if (!opened) return 0;
        read_cursor = 0;
        read_buf.clear();

        const char* ptr = static_cast<const char*>(buffer);
        size_t k = 0;
        while (k < size) {
            if (write_buf.size() >= BufferSize) {
                flush();
            }
            const size_t to_copy = std::min(size - k, BufferSize - write_buf.size());
            write_buf.insert(write_buf.end(), ptr + k, ptr + k + to_copy);
            k += to_copy;
        }
        return k;
    }

public:
    BufferedReaderWriter(ReaderWriter* source) : source(source) {
        if (!source) throw std::invalid_argument("Source cannot be null");
    }

    ~BufferedReaderWriter() override {
        BufferedReaderWriter::close();
    }

    void close() override {
        if (opened) {
            flush();
            source->close();
            opened = false;
        }
    }

    void seek(const size_t pos) override {
        if (!opened) return;
        flush();
        read_cursor = 0;
        read_buf.clear();
        source->seek(pos);
        eof = false;
    }
};

int main() {
    StringReaderWriter str_io("ByeBye World!");
    str_io.seek(0);
    str_io.write(std::string("Hello,"));
    str_io.seek(0);
    std::string out_str;
    str_io.read(out_str, 20);
    assert(out_str == "Hello, World!");

    str_io.seek(str_io.write(42.5));
    str_io.seek(13);
    double out_double = 0;
    str_io.read(out_double);
    assert(out_double == 42.5);


    FILE* temp = fopen("test", "wb");
    if (temp) fclose(temp);

    FileReaderWriter file_io("test");
    int num = 42;
    file_io.write(num);
    file_io.write(std::string("file data"));
    file_io.seek(0);
    int read_num = 0;
    file_io.read(read_num);
    assert(read_num == num);
    std::string read_file_str;
    file_io.read(read_file_str, 9);
    assert(read_file_str == "file data");



    file_io.seek(0);
    BufferedReaderWriter<5> buffered_io(&file_io);
    buffered_io.write(std::string("buffer feferr"));
    buffered_io.seek(0);
    std::string buf_result;
    buffered_io.read(buf_result, 13);
    assert(buf_result == "buffer feferr");
}
