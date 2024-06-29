#pragma once

#include "IDebugStream.h"
#include "IDebugSource.h"
#include "IDebugOutput.h"
#include <list>

class IDebugStream;
class IDebugSource;
class IDebugOutput;

/**
 *	The high-level debug log manager
 *	
 *	All debug sources, streams, and outputs must be registered with this class.
 */
class IDebugManager : public ISingleton <IDebugManager>
{
	public:
				IDebugManager();
				~IDebugManager();
		
		void	RegisterStream(IDebugStream * inStream);
		void	RegisterSource(IDebugSource * inSource);
		void	RegisterOutput(IDebugOutput * inOutput);
		
		void	FlushStreams(void);
	
	private:
		std::list <IDebugStream *>	streamList;
		std::list <IDebugSource *>	sourceList;
		std::list <IDebugOutput *>	outputList;
};

#include "IDebugTextOutput.h"

extern IDebugTextOutput gDebugLog;

