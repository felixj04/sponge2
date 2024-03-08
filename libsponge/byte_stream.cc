#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {capa_city = capacity; }

size_t ByteStream::write(const string &data) {
    size_t length = min(remaining_capacity(), data.length());
    buffer.append(data.substr(0, length));
    written_size += length;
    return length;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t length = min(buffer.length(), len);
    return buffer.substr(0, length);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len)
{
    size_t length = min(buffer.length(), len);
    buffer.erase(0, len);
    read_size += length;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    std::string result = peek_output(len);
    pop_output(len);
    return result;
}

void ByteStream::end_input() {is_ended = true;}

bool ByteStream::input_ended() const { return is_ended; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return buffer_empty() && input_ended(); }

size_t ByteStream::bytes_written() const { return written_size; }

size_t ByteStream::bytes_read() const { return read_size; }

size_t ByteStream::remaining_capacity() const { return capa_city - buffer.size(); }
