#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    DUMMY_CODE(seg);
    TCPHeader header = seg.header();
    bool syn = header.syn;
    bool fin = header.fin;
    bool eof = false;
    string data = seg.payload().copy();

    if(syn ==0 && is_syn == 0)
	return;
    if(is_syn == 0){
        is_syn = true;
    }

    if(syn  == 1 && is_syn == 0){
        is_syn = true;
	i_seqno = header.seqno;
	if(header.fin == 1){
	    is_fin = true;
	    eof = true;
	}
	_reassembler.push_substring(data, 0, eof);
	return;
    }
    if(fin == 1 || is_fin == 1){
        is_fin = true;
	eof = true;
    }
    uint64_t stream_idx = unwrap(header.seqno, i_seqno, _reassembler.ack_idx())- is_syn;
    _reassembler.push_substring(data, stream_idx, eof);
    
}

optional<WrappingInt32> TCPReceiver::ackno() const 
{
    if(is_syn == 0)
	return nullopt;
    bool check_reas_fin = _reassembler.empty() && is_fin;
    return wrap(_reassembler.ack_idx() + check_reas_fin + 1, i_seqno);
}

size_t TCPReceiver::window_size() const {return _capacity - _reassembler.stream_out().buffer_size();}
