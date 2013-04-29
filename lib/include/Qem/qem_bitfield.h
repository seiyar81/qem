#ifndef BIT_FIELD_H_INCLUDED
#define BIT_FIELD_H_INCLUDED

#include <cstring>

namespace Qem
{

	template<unsigned int SIZE>
	class BitField
	{
		static const unsigned int m_size			= SIZE;
		static const unsigned int m_bitsPerCell		= 8 * sizeof(char);
		static const unsigned int m_cellCount		= 1 + (SIZE / m_bitsPerCell);
	public:
								BitField();

		void					setBit(unsigned int index, bool value);
		void					reset(bool value);
		bool					getBit(unsigned int index) const;

	private:
		void					extractCellAndBit(unsigned int index, unsigned int& cell, unsigned int& bit) const;

	private:
		char					m_field[m_cellCount];
	};

	template<unsigned int SIZE>
	BitField<SIZE>::BitField()
	{
		for(unsigned int i = 0; i < m_cellCount; ++i)
			m_field[i] = 0;
	}

	template<unsigned int SIZE>
	void BitField<SIZE>::setBit(unsigned int index, bool value)
	{
		unsigned int cell = 0, bit = 0;
		extractCellAndBit(index, cell, bit);

		if(value)	m_field[cell] |= bit;
		else			m_field[cell] &= ~bit;
	}

	template<unsigned int SIZE>
	void BitField<SIZE>::extractCellAndBit(unsigned int index, unsigned int& cell, unsigned int& bit) const
	{
		QEM_ASSERT(index < SIZE, "");
		cell = index / m_bitsPerCell;
		QEM_ASSERT(cell < m_cellCount, "");
		const unsigned int bitPosition = (index - cell * m_bitsPerCell);
		QEM_ASSERT(bitPosition < m_bitsPerCell, "");
		bit = (1 << bitPosition);
	}

	template<unsigned int SIZE>
	void BitField<SIZE>::reset(bool value)
	{
		const char newValue = (value) ? (255) : (0);
		std::memset(m_field, newValue, m_cellCount * sizeof(char));
	}

	template<unsigned int SIZE>
	bool BitField<SIZE>::getBit(unsigned int index) const
	{
		unsigned int cell = 0, bit = 0;
		extractCellAndBit(index, cell, bit);

		return (m_field[cell] & bit) != 0;
	}

}

#endif// BIT_FIELD_H_INCLUDED

