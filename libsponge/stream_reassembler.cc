#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),
_check(string(_capacity, 0)), _buffer(string(_capacity, 0)){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t initial_loc = _output.bytes_written();
    size_t last_loc = initial_loc + _output.buffer_size() + _capacity;
    size_t num_assembled = 0;

    for(size_t i = max(initial_loc, index); i<min(last_loc, index + data.length()); i++){
	if(_check[i - initial_loc] == 0){
	    _buffer[i - initial_loc] = data[i - index];
	    _check[i - initial_loc] = 1;
	    num_unassembled++;
	}
    }

    for(size_t i = 0; i<_capacity; i++){
        if(_check[i] == 0)
	    break;
	num_assembled++;
    }

    _output.write(_buffer.substr(0, num_assembled));
    _buffer.erase(0, num_assembled);
    _buffer.append(num_assembled, ' ');
    _check.erase(0, num_assembled);
    _check.append(num_assembled, 0);
    num_unassembled = num_unassembled - num_assembled;

    if(eof && index + data.length() <= last_loc && num_unassembled == 0)
    	_output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const { return num_unassembled; }

bool StreamReassembler::empty() const { return num_unassembled == 0; }
