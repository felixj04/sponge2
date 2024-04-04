#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const { return flight; }

void TCPSender::fill_window() {
    if(is_syn == 0){
        is_syn = 1;
	TCPSegment seg;
	seg.header().syn = 1;
	seg.header().seqno = wrap(_next_seqno, _isn);
	_next_seqno += seg.length_in_sequence_space();
	flight += seg.length_in_sequence_space();
	if(is_syn == 1){
	    free_space -= seg.length_in_sequence_space();
	}
	_segments_out.push(seg);
	segments.push(seg);
	if(is_timer == 0){
	    is_timer = 1;
	    time_el = 0;
	}
	return;
    }
    if((segments.empty() == 0 && segments.front().header().syn == 1) || is_fin == 1)
	return;

    if(window_size == 1){
	while(free_space == 1){
	    TCPSegment seg;
	    size_t payload = min({_stream.buffer_size(),free_space, TCPConfig::MAX_PAYLOAD_SIZE});
	    seg.payload() = Buffer{_stream.read(payload)};
	    if(_stream.eof() && free_space > payload){
	    	seg.header().fin = 1;
		is_fin = 1;
	    }
	    seg.header().seqno = wrap(_next_seqno, _isn);
	    _next_seqno = _next_seqno + seg.length_in_sequence_space();
	    flight += seg.length_in_sequence_space();
	    if(is_syn ==1 ){
	    	free_space = free_space - seg.length_in_sequence_space();
	    }
	    _segments_out.push(seg);
	    segments.push(seg);
	    if(is_timer == 0){
	    	is_timer = 1;
		time_el = 0;
	    }
	    if(_stream.buffer_empty() == 1){
	    	break;
	    }
	}
    }
    else if(free_space == 0){
    	TCPSegment seg;
	if(_stream.eof() == 1){
	    seg.header().fin = 1;
	    is_fin = 1;
	    seg.header().seqno = wrap(_next_seqno, _isn);
	    _next_seqno += seg.length_in_sequence_space();
	    flight += seg.length_in_sequence_space();
	    if(is_syn == 1){
	    	free_space -= seg.length_in_sequence_space();
	    }
	    _segments_out.push(seg);
	    segments.push(seg);
	    if(is_timer == 0){
	    	is_timer = 1;
		time_el = 0;
	    }
	}
	else if(_stream.buffer_empty() == 0){
	    seg.payload() = Buffer{_stream.read(1)};
	    seg.header().seqno = wrap(_next_seqno, _isn);
	    _next_seqno += seg.length_in_sequence_space();
	    flight += seg.length_in_sequence_space();
	    if(is_syn == 1){
	   	free_space -= seg.length_in_sequence_space();
	    }
	    _segments_out.push(seg);
	    segments.push(seg);
	    if(is_timer == 0){
	    	is_timer =1 ;
		time_el = 0;
	    }
	}
    }
}


//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t _window_size) {
    uint64_t unwrapped_ackno = unwrap(ackno, _isn, _next_seqno);
    bool is_ack_valid = 0;
    if(segments.empty()){
    	if(unwrapped_ackno <= _next_seqno){
	    is_ack_valid = 1;
	}
	if((unwrapped_ackno <= _next_seqno) && (unwrapped_ackno >= unwrap(segments.front().header().seqno, _isn, _next_seqno))){
	    is_ack_valid = 1;
	}	
    }
    if(is_ack_valid == 0){
    	return;
    }
    window_size = _window_size;
    free_space = _window_size;
    while(segments.empty() == 0){
    	TCPSegment seg;
	seg = segments.front();
	if(unwrap(seg.header().seqno, _isn, _next_seqno) + seg.length_in_sequence_space() <= unwrapped_ackno){
	    flight -= seg.length_in_sequence_space();
	    segments.pop();
	    time_el = 0;
	    rto = _initial_retransmission_timeout;
	    retransmission = 0; 
	}
	else{
	    break;
	}
    }
    if(segments.empty() == 0){
    	free_space = unwrapped_ackno + _window_size - unwrap(segments.front().header().seqno, _isn, _next_seqno) - flight;
    }
    if(flight == 0){
    	is_timer = false;
    }
    fill_window();
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    if(is_timer == 0){
    	return;
    }
    time_el += ms_since_last_tick;
    if(time_el >= rto){
    	segments.push(segments.front());
	if(window_size || segments.front().header().syn){
	    ++retransmission;
	    rto <<= 1;
	}
	time_el = 0;
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return retransmission; }

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.push(seg);
}
