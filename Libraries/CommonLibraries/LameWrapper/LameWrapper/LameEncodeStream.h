// LameEncodeStream.h

#pragma once

using namespace System;
using namespace System::IO;

#include "lame.h"

namespace LameWrapper
{
	public ref class LameEncodeStream : public Stream
	{
	public:
		LameEncodeStream(Stream^ innerStream);
	
		~LameEncodeStream();
		!LameEncodeStream();

		void InitialLame();
		virtual int Read(array<Byte>^ buffer, int offset, int count) override;
		virtual __int64 Seek(__int64 offset, SeekOrigin origin) override;
		virtual void SetLength(__int64 value) override;
		virtual void Write(array<Byte>^ buffer, int offset, int count) override;  
		virtual void Flush() override;

		virtual property bool CanRead
		{
			bool get() override
			{ return false; }
		}
		virtual property bool CanSeek
		{
			bool get() override
			{ return false; } 
		}
		virtual property bool CanWrite
		{
			bool get() override
			{ return true; }
		}
		virtual property __int64 Length
		{
			__int64 get() override
			{ return 0; }
		}
		virtual property __int64 Position
		{
			__int64 get() override
			{ return 0; }
			void set(__int64 value) override
			{ }
		}
	protected:
		
	private:
		void ExtendBuffer(int bufferLen);
		void FullPcmBuf(array<Byte>^ buffer, int offset, int count);
		
	private:
		Stream^ _innerStream;
		lame_global_flags *_pLameGF;
		array<Int16>^ _pcmBuf;
		int _sampleCount;
		array<Byte>^ _mp3Buf;
	};
}

