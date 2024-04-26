#include "tcp_connection.hh"
#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return time_cnt; }

void TCPConnection::segment_received(const TCPSegment &seg) {
	bool issend = false;
	time_cnt = 0;
	if(seg.header().rst == 1){
	    _sender.stream_in().set_error();
	    _receiver.stream_out().set_error();
	    is_active = false;
	}
	_receiver.segment_received(seg); //giving the segment to receiver
	if(seg.header().ack == 1){
		_sender.ack_received(seg.header().ackno, seg.header().win);
		while(!_sender.segments_out().empty()){
			TCPSegment segment = _sender.segments_out().front();
			_sender.segments_out().pop();
			setting_ack_window(segment);
			_segments_out.push(segment);
		}
	}
	if((_receiver.unassembled_bytes() == 0 && _receiver.stream_out().input_ended()) && !_sender.stream_in().eof()){
		_linger_after_streams_finish = false;
	}
	if(seg.length_in_sequence_space() >0 ){
		_sender.fill_window();
		while(!_sender.segments_out().empty()){
			issend = true;
			TCPSegment segment = _sender.segments_out().front();
			_sender.segments_out().pop();
			setting_ack_window(segment);
			_segments_out.push(segment);
		}
		if(!issend){
			_sender.send_empty_segment();
			TCPSegment ACK = _sender.segments_out().front();
			_sender.segments_out().pop();
			setting_ack_window(ACK);
			_segments_out.push(ACK);
		}
	}
	return;
}

bool TCPConnection::active() const { return is_active; }

size_t TCPConnection::write(const string &data) {
    if (data.size() == 0)
	    return 0;
    size_t write_size = _sender.stream_in().write(data);
    _sender.fill_window();
    while(!_sender.segments_out().empty()){
    	TCPSegment segment = _sender.segments_out().front();
	_sender.segments_out().pop();
	setting_ack_window(segment);
	_segments_out.push(segment);
    }
    return write_size;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
	time_cnt = time_cnt + ms_since_last_tick;
	_sender.tick(ms_since_last_tick);
	
	return;
}

void TCPConnection::end_input_stream() {
	_sender.stream_in().end_input();
	_sender.fill_window();
	while(!_sender.segments_out().empty()){
		TCPSegment segment = _sender.segments_out().front();
		_sender.segments_out().pop();
		setting_ack_window(segment);
		_segments_out.push(segment);
	}
}

void TCPConnection::connect() {
    _sender.fill_window();
    while(!_sender.segments_out().empty()){
    	TCPSegment segment = _sender.segments_out().front();
	_sender.segments_out().pop();
	setting_ack_window(segment);
	_segments_out.push(segment);
    }
}

void TCPConnection::setting_ack_window(TCPSegment &segment){
	optional<WrappingInt32> ackno = _receiver.ackno();
	if(ackno.has_value()){
		segment.header().ack = true;
		segment.header().ackno = ackno.value();
	}
	size_t window = _receiver.window_size();
	segment.header().win = static_cast<uint16_t>(window);
}
TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";
	    _sender.stream_in().set_error();
	    _receiver.stream_out().set_error();
	    _sender.send_empty_segment();
	    TCPSegment RST = _sender.segments_out().front();
	    _sender.segments_out().pop();
	    setting_ack_window(RST);
	    RST.header().rst = true;
	    _segments_out.push(RST);
	    is_active = false;
            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
