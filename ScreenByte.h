class ScreenByte
{
public:
	ScreenByte(uint32_t mode) : m_mode(mode),m_byte(0),m_offset(0) {}
	ScreenByte() = delete;
	~ScreenByte() {}

	bool addPixel(uint8_t pixelIndex)
	{
		// TODO: This currently assumes mode 5
		switch (pixelIndex)
		{
		case 0:
			// Nothing
			break;

		case 1:
			m_byte |= 0b011;
			break;

		case 2:
			m_byte |= 0b110;
			break;

		case 3:
			m_byte |= 0b111;
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
			return true;
		}
		else
		{
			return false;
		}
	}

	uint8_t getByte() const
	{
		return m_byte;
	}

	uint32_t getPixelsPerByte() const
	{
		switch (m_mode)
		{
		case 2:
			return 2;
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
