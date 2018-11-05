#ifndef INCLUDE_SIMPLEPROTOCOLPARSER_H_
#define INCLUDE_SIMPLEPROTOCOLPARSER_H_

#include <memory>
#include "parserState.h"

class SimpleProtocolParser
{
	public:
		SimpleProtocolParser();
		virtual ~SimpleProtocolParser(){}

		void receiveChar(char received);
		void setState(std::shared_ptr<ParserState> state);

		virtual void receptionStart() = 0;
  		virtual void receiveData(char received) = 0;
		virtual void appendReceivedData() = 0;
		virtual void receptionCompleted() = 0;

	private:
		std::shared_ptr<ParserState> pState;
};


#endif /* INCLUDE_SIMPLEPROTOCOLPARSER_H_ */
