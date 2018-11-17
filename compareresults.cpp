#include "compareresults.h"
#include <QXmlStreamReader>
#include <stack>

namespace resultXmlCompare
{

QString compareResults::compare(QString resultOld, QString resultNew)
{
	std::stringstream messages	;
	bool same = compare(resultOld.toStdString(), resultNew.toStdString(), messages);

	return (same ? "Succes!\n" : "Failure...\n") + QString::fromStdString(messages.str());
}

result compareResults::convertXmltoResultStruct(QString resultXml, std::stringstream & msgs)
{
	QXmlStreamReader xml(resultXml);


	enum class stateType { nothing, result, table, head, body, foot, row, headercell, bodycell };

	typedef QXmlStreamReader::TokenType ttype;
	typedef std::stack<stateType>		typeStack;

	result res;
	typeStack levels;
	levels.push(stateType::result);

	auto curLinePos = [&](){ return "(line: " + std::to_string(xml.lineNumber()) + " col: " + std::to_string(xml.columnNumber()) + ")"; };
	auto curBlock	= [&]()
	{
		typeStack		levelCopy			= levels;
		tableBlock *	returnMe			= res.curTable().body(); // body is default

		auto			checkSaveAndPopTop	= [&](stateType checkMe)
		{
			bool	isType = levelCopy.top() == checkMe;

			if(isType)
				levelCopy.pop();
			return isType;
		};

		checkSaveAndPopTop(stateType::headercell);
		checkSaveAndPopTop(stateType::bodycell);
		checkSaveAndPopTop(stateType::row);

		if(levelCopy.top() == stateType::head)
			returnMe = res.curTable().head();
		else if(levelCopy.top() == stateType::foot)
			returnMe = res.curTable().foot();

		return returnMe;
	};

	auto topIs = [&](stateType checkMe) { return levels.top() == checkMe; };

	while (!xml.atEnd())
	{
		switch(xml.readNext())
		{
		case ttype::StartElement:
			if(xml.name() == "table")
			{
				res.genTable();
				levels.push(stateType::table);
			}

			if(xml.name() == "thead")
			{
				if(!topIs(stateType::table))
					msgs << "found table head " << curLinePos() << " outside of table!\n";

				levels.push(stateType::head);
			}

			if(xml.name() == "tbody")
			{
				if(!topIs(stateType::table))
					msgs << "found table body " << curLinePos() << " outside of table!\n";

				levels.push(stateType::body);
			}

			if(xml.name() == "tfoot")
			{
				if(!topIs(stateType::table))
					msgs << "found table footer " << curLinePos() << " outside of footer!\n";

				levels.push(stateType::foot);
			}

			if(xml.name() == "tr")
			{
				if(!(topIs(stateType::head) || topIs(stateType::body) || topIs(stateType::foot)))
					msgs << "found row " << curLinePos() << " outside of table head, body or footer!\n";

				levels.push(stateType::row);

				curBlock()->genRow();
			}

			if(xml.name() == "th")
			{
				if(!topIs(stateType::row))
					msgs << "found headercell " << curLinePos() << " outside of row!\n";

				levels.push(stateType::headercell);

				curBlock()->curRow().genHeaderCell();
			}

			if(xml.name() == "td")
			{
				if(!topIs(stateType::row))
					msgs << "found bodycell " << curLinePos() << " outside of row!\n";

				levels.push(stateType::bodycell);

				curBlock()->curRow().genBodyCell();
			}
			break;

		case ttype::Characters:
			if(!xml.isWhitespace() && (topIs(stateType::headercell) || topIs(stateType::bodycell)))
			{
				tableBlock * curBlk = curBlock();
				curBlk->curRow().curCell().addToValue(xml.text().toString().trimmed().toStdString());
			}

			break;

		case ttype::EndElement:
			if(xml.name() == "th")
			{
				if(!topIs(stateType::headercell))
					msgs << "found end of headercell " << curLinePos() << " outside of headercell!\n";
				levels.pop();
			}

			if(xml.name() == "td")
			{
				if(!topIs(stateType::bodycell))
					msgs << "found end of bodycell " << curLinePos() << " outside of bodycell!\n";
				levels.pop();
			}

			if(xml.name() == "tr")
			{
				if(!topIs(stateType::row))
					msgs << "found end of row " << curLinePos() << " outside of row!\n";
				levels.pop();
			}

			if(xml.name() == "thead")
			{
				if(!topIs(stateType::head))
					msgs << "found table head " << curLinePos() << " outside of table!\n";
				levels.pop();
			}

			if(xml.name() == "tbody")
			{
				if(!topIs(stateType::body))
					msgs << "found end of body of table " << curLinePos() << " outside of body!\n";
				levels.pop();
			}

			if(xml.name() == "tfoot")
			{
				if(!topIs(stateType::foot))
					msgs << "found end of footer of table " << curLinePos() << " outside of footer!\n";
				levels.pop();
			}

			if(xml.name() == "table")
			{
				if(!topIs(stateType::table))
					msgs << "found end of table " << curLinePos() << " outside of table!\n";
				levels.pop();
			}
			break;

		default:
			//ignore
			break;
		}
	}

	if(xml.hasError())
	{
		msgs << "xml hadError: " << xml.errorString().toStdString() << "!\n";
	}

	return res;
}

bool compareResults::compare(std::string resultOld, std::string resultNew, std::stringstream & messages)
{
	messages << "Converting old\n";
	result oldRes = convertXmltoResultStruct(QString::fromStdString(resultOld), messages);

	messages << "\nConverted to:\n" << oldRes.toString() << "\n";

	messages << "Converting new\n";
	result newRes = convertXmltoResultStruct(QString::fromStdString(resultNew), messages);
	messages << "\nConverted to:\n" << newRes.toString() << "\n";

	return oldRes == newRes;
}


}
