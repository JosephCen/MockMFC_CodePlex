using namespace System;
using namespace System::IO;

#include "StdAfx.h"
#include "LameEncodeStream.h"
using namespace LameWrapper;
#include "lame.h"


LameEncodeStream::LameEncodeStream(Stream^ innerStream)
{
	_pLameGF = NULL;

	if (nullptr == innerStream)
		throw gcnew ArgumentNullException("innerStream");
    if (!innerStream->CanWrite)
		throw gcnew ArgumentException("innerStream should be a can-write stream.", "innerStream");
	_innerStream = innerStream;

	this->ExtendBuffer(4096);
}

LameEncodeStream::~LameEncodeStream()
{
	this->Flush();
	_innerStream->Close();
	_innerStream = nullptr;

	this->!LameEncodeStream();
}

LameEncodeStream::!LameEncodeStream()
{
	if (NULL != _pLameGF)
	{
		lame_close(_pLameGF);
		_pLameGF = NULL;
	}
}

void LameEncodeStream::ExtendBuffer(int bufferLen)
{
	if (_pcmBuf == nullptr || bufferLen > _pcmBuf->Length)
		_pcmBuf = gcnew array<Int16>(bufferLen);
	if (_mp3Buf == nullptr || bufferLen > _mp3Buf->Length)
		_mp3Buf = gcnew array<Byte>(bufferLen);
}

void LameEncodeStream::FullPcmBuf(array<Byte>^ buffer, int offset, int count)
{
	if (buffer->Length < offset + count)
		throw gcnew ArgumentException("Length of buffer is less than offset + count.", "buffer");

	// Single channel & 16bit
	if (count % 2 != 0)
		throw gcnew ArgumentException("count should be a multiple of 2.", "count");

	_sampleCount = count / 2;

	this->ExtendBuffer(_sampleCount * 2);
	for (int i = 0; i < _sampleCount; ++i)
	{
		_pcmBuf[i*2] = BitConverter::ToInt16(buffer, offset + i*2);
		_pcmBuf[i*2 + 1] = _pcmBuf[i*2];
	}
}

void LameEncodeStream::Write(array<Byte>^ buffer, int offset, int count)
{
	this->FullPcmBuf(buffer, offset, count);

	pin_ptr<short int> pPcmBuf = &_pcmBuf[0];
	pin_ptr<unsigned char> pMp3Buf = &_mp3Buf[0];
	int mp3DataLen = 0;

	mp3DataLen = lame_encode_buffer_interleaved(_pLameGF, pPcmBuf, _sampleCount, pMp3Buf, _mp3Buf->Length);
	_innerStream->Write(_mp3Buf, 0, mp3DataLen);
    System::Diagnostics::Debug::WriteLine(String::Format("mp3 data len: {0}", mp3DataLen));
}

void LameEncodeStream::Flush()
{
	pin_ptr<unsigned char> pMp3Buf = &_mp3Buf[0];
	int mp3DataLen = 0;

	mp3DataLen = lame_encode_flush(_pLameGF, pMp3Buf, _mp3Buf->Length);
	_innerStream->Write(_mp3Buf, 0, mp3DataLen);
	_innerStream->Flush();
}

void LameEncodeStream::InitialLame()
{
	_pLameGF = lame_init();
	
	lame_set_in_samplerate(_pLameGF, 22050); // 22.05kHz

	// Setting Channels
	lame_set_mode(_pLameGF, MONO);
	lame_set_num_channels(_pLameGF, 1);

	// VbrTag
	lame_set_VBR(_pLameGF, vbr_abr);
	lame_set_VBR_mean_bitrate_kbps(_pLameGF, 24); // 24kbps

	// Set resample rate
    lame_set_out_samplerate(_pLameGF, 22050); // 22.05kHz

    lame_set_findReplayGain(_pLameGF, 1);
	lame_set_write_id3tag_automatic(_pLameGF, 0); //Dont write id3tag, will write it myself

	if (lame_init_params(_pLameGF) == -1)
		throw gcnew InvalidOperationException("Parameters failed to initialize properly in lame!");

	//pin_ptr<unsigned char> pMp3Buf = &_mp3Buf[0];
	//int imp3DataLen = 0;

	//imp3DataLen = lame_get_id3v2_tag(_pLameGF, pMp3Buf, _mp3Buf->Length);
}

int LameEncodeStream::Read(array<Byte>^ buffer, int offset, int count)
{
	throw gcnew InvalidOperationException("Read method cannot be called against a LameEncodeStream instance.");

	return 0;
}

__int64 LameEncodeStream::Seek(__int64 offset, SeekOrigin origin)
{
	throw gcnew InvalidOperationException("Seek method cannot be called against a LameEncodeStream instance.");

	return 0;
}

void LameEncodeStream::SetLength(__int64 value)
{
	throw gcnew InvalidOperationException("SetLength method cannot be called against a LameEncodeStream instance.");
}
