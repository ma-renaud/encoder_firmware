#ifndef INCLUDE_PROCESSORSTATE_H_
#define INCLUDE_PROCESSORSTATE_H_

class ParserState
{
	public :
		virtual ~ParserState(){};
		virtual void receiveChar(char received) = 0;
};

#endif /* INCLUDE_PROCESSORSTATE_H_ */
