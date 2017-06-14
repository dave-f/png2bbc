class ScreenByte
{
public:
	ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
	ScreenByte() = delete;
	~ScreenByte() {}

	// TODO: This currently assumes mode 5
	bool addPixel(uint8_t pixelValue)
	{
		// Then or in new pixel (assumes mode 5 here)
		switch (pixelValue)
		{
		case 0:
			break;

		case 1:
			m_byte |= (0b00001);
			break;

		case 2:
			m_byte |= (0b10000);
			break;

		case 3:
			m_byte |= (0b10001);
			break;

		case 4:
		case 5:
		case 6:
		case 7:
		default:
			throw std::runtime_error("Bad pixel for this mode");
			break;
		}

		if (++m_offset == getPixelsPerByte())
		{
			m_offset = 0;

			// This byte is done
			return true;
		}
		else
		{
			m_byte <<= 1;

			return false;
		}
	}

	uint8_t readByte()
	{
		uint8_t r = m_byte;
		m_byte    = 0;

		return r;
	}

	uint32_t getPixelsPerByte() const
	{
		switch (m_mode)
		{
		case 1:
			return 2;
			break;

		case 2:
			return 2;
			break;

		case 3:
			return 0;
			break;

		case 4:
			return 0;
			break;

		case 5:
			return 4;
			break;

		default:
			return 0;
			break;
		}
	}

private:
	uint8_t m_byte;
	uint32_t m_offset;
	uint32_t m_mode;
};
