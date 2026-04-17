#include <string>

class IO {
    bool opened = false;
    bool eof = false;
    size_t cursor = 0;

    void close() {
        if (opened) {
            opened = false;
        }
    }
};

class Reader: public virtual IO {
    template<typename T>
    void read(T value, size_t count) {
        readBytes(sizeof(value), count);
    }

    virtual void readBytes(void* buffer, size_t size, size_t count) {}
};

class Writer: public virtual IO {
    template<typename T>
    void write(T value, size_t count) {
        writeBytes(value, sizeof(value), count);
    }

    virtual void writeBytes(char value, size_t size, size_t count) {}
};

class ReaderWriter: public virtual Reader, public virtual Writer {
    virtual void seek(size_t pos);
};

class StringReaderWriter: public virtual ReaderWriter {
    std::string data;
    void writeBytes(size_t size, size_t count) override {
        data[cursor] = (char)
    }
};

class FileReaderWriter: public virtual ReaderWriter {}
