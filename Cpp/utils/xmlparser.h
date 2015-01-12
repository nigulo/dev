// Class automatically generated by Dev-C++ New Class wizard

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "base/object.h" // inheriting class's header file
#include "base/linkedlist.h"
#include "base/string.h"

using namespace base;

namespace utils {

/**
 * A primitive XML parser
 */
class XmlParser : public Object
{

    public:
    class XmlElement
    {
        public:
            XmlElement(const String& rType);
            XmlElement(const String& rType, const String& rName, const String& rParams, String& data, int startIndex, int endIndex);
            
            void AddParameter(const String& rName, const String& rValue = "");
            void AddSubElement(XmlElement* pElement);
            void SetData(const String& rData);
            
            String GetParameter(const String& rParamName) const {
            	return mParams.GetProperty(rParamName);
            }
            
            List<XmlElement*>& GetSubElements() {
            	return mSubElements;
            }
            
            virtual ~XmlElement();
            void Parse();
            String ToString() const;

    		void Save(const String& rFileName) const;
        private:
            XmlElement* FindNextSubElement(int* pStartIndex);
            String ToString(const String& indent) const;
        public:
            String mName;
            String mParams;
            String mType;
            LinkedList<XmlElement*> mSubElements;
            String mData;
        private:
            int mStartIndex;
            int mEndIndex;
            String* mpData;
    };
    
	public:
		// class constructor
		XmlParser(const String& rFileName = "");
		XmlElement* Load(const String& rFileName = "");
		// class destructor
		virtual ~XmlParser();
	private:
        XmlElement* mpData;
};
}
#endif // XMLPARSER_H
