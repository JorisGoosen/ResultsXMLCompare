#include "resultscomparetable.h"
#include <sstream>

namespace resultXmlCompare
{

bool tableCell::isEqual(const tableCell & other) const
{
	return isHeaderCell == other.isHeaderCell && _value == other._value;
}

bool tableRow::isEqual(const tableRow & other) const
{
	if(cells.size() != other.cells.size())
		return false;

	for(size_t cell=0; cell < cells.size(); cell++)
		if(cells[cell] != other.cells[cell])
			return false;

	return true;
}


void tableRow::genHeaderCell()
{
	genCell();
	curCell().setHeader(true);
}

void tableRow::genBodyCell()
{
	genCell();
	curCell().setHeader(false);
}

void tableRow::genCell()
{
	cells.resize(cells.size() + 1);
}

tableCell & tableRow::curCell()
{
	if(cells.size() == 0)
		genBodyCell();

	return cells[cells.size() - 1];
}

bool tableBlock::isEqual(const tableBlock & other) const
{
	if(rows.size() != other.rows.size())
		return false;

	for(size_t row=0; row < rows.size(); row++)
		if(rows[row] != other.rows[row])
			return false;

	return true;
}

void tableBlock::genRow()
{
	rows.resize(rows.size() + 1);
}

tableRow & tableBlock::curRow()
{
	if(rows.size() == 0)
		genRow();

	return rows[rows.size() - 1];
}


bool table::isEqual(const table & other) const
{
	return _head == other._head && _body == other._body && _foot == other._foot;
}


bool result::isEqual(const result & other) const
{
	if(resultTables.size() != other.resultTables.size())
		return false;

	for(size_t resultTable=0; resultTable < resultTables.size(); resultTable++)
		if(resultTables[resultTable] != other.resultTables[resultTable])
			return false;

	return true;
}

void result::genTable()
{
	resultTables.resize(resultTables.size() + 1);
}

table & result::curTable()
{
	if(resultTables.size() == 0)
		genTable();

	return resultTables[resultTables.size() - 1];
}

std::string tableCell::toString()
{
	return (isHeaderCell ? "h" : "") + _value;
}

std::string	tableRow::toString()
{
	std::stringstream out;

	out << "\t\t\tRow: |";
	for(auto & cell : cells)
		out << cell.toString() << " | ";
	out << "\n";

	return out.str();
}

std::string	tableBlock::toString()
{
	std::stringstream out;

	out << "\t\tRows:\n";

	for(auto & row : rows)
		out << row.toString() << " | ";

	return out.str();
}

std::string	table::toString()
{
	std::stringstream out;

	out << "\tHeader:\n"	<< _head.toString() << "\n";
	out << "\tBody:\n"		<< _body.toString() << "\n";
	out << "\tFooter\n: "	<< _foot.toString() << "\n";

	return out.str();
}

std::string	result::toString()
{
	std::stringstream out;

	out << "Tables:\n";

	for(auto & table : resultTables)
		out << table.toString() << "\n";

	return out.str();
}


}
