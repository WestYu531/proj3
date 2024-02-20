#include "XMLWriter.h"
#include "StringUtils.h"
#include <stack>

struct CXMLWriter::SImplementation {
	std::shared_ptr<CDataSink> DSink;
	std::stack<SXMLEntity> DEntities;

	SImplementation(std::shared_ptr < CDataSink > sink) {
		DSink = sink;
	}

	~SImplementation() {

	}

	bool Flush() {
		while (!DEntities.empty()) {
			WriteEntity(DEntities.top());
		}

		return true;
	}

	bool WriteEntity(const SXMLEntity& entity) {
		std::string converted_string;

		switch (entity.DType) {
		case (SXMLEntity::EType::StartElement): {
			DSink->Put('<');
			DSink->Write(std::vector<char>(entity.DNameData.begin(), entity.DNameData.end()));
			for (auto& Attribute : entity.DAttributes) {
				DSink->Put(' ');
				converted_string = ReplaceSpecialChars(std::get<0>(Attribute));
				DSink->Write(std::vector<char>(converted_string.begin(), converted_string.end()));
				DSink->Put('=');
				DSink->Put('\"');
				converted_string = ReplaceSpecialChars(std::get<1>(Attribute));
				DSink->Write(std::vector<char>(converted_string.begin(), converted_string.end()));
				DSink->Put('\"');
			}
			DSink->Put('>');

			SXMLEntity entity_copy = { .DType = SXMLEntity::EType::EndElement, .DNameData = entity.DNameData,
										.DAttributes = entity.DAttributes };
			DEntities.push(entity_copy);
			break;
		}
		case (SXMLEntity::EType::EndElement):
			if (DEntities.empty()) {
				return false;
			}
			if (DEntities.top().DNameData == entity.DNameData) {
				DSink->Put('<');
				DSink->Put('/');
				DSink->Write(std::vector<char>(entity.DNameData.begin(), entity.DNameData.end()));
				DSink->Put('>');
				DEntities.pop();
				break;
			}
			else {
				return false;
			}
		case (SXMLEntity::EType::CharData):
			converted_string = ReplaceSpecialChars(entity.DNameData);
			DSink->Write(std::vector<char>(converted_string.begin(), converted_string.end()));
			break;
		case (SXMLEntity::EType::CompleteElement):
			DSink->Put('<');
			DSink->Write(std::vector<char>(entity.DNameData.begin(), entity.DNameData.end()));
			for (auto& Attribute : entity.DAttributes) {
				DSink->Put(' ');
				converted_string = ReplaceSpecialChars(std::get<0>(Attribute));
				DSink->Write(std::vector<char>(converted_string.begin(), converted_string.end()));
				DSink->Put('=');
				DSink->Put('\"');
				converted_string = ReplaceSpecialChars(std::get<1>(Attribute));
				DSink->Write(std::vector<char>(converted_string.begin(), converted_string.end()));
				DSink->Put('\"');
			}
			DSink->Put('/');
			DSink->Put('>');
			break;
		}

		return true;
	}

	std::string ReplaceSpecialChars(const std::string& input) {
		std::string new_string = input;

		new_string = StringUtils::Replace(new_string, "&", "&amp;");
		new_string = StringUtils::Replace(new_string, "\"", "&quot;");
		new_string = StringUtils::Replace(new_string, "'", "&apos;");
		new_string = StringUtils::Replace(new_string, "<", "&lt;");
		new_string = StringUtils::Replace(new_string, ">", "&gt;");

		return new_string;
	}
};

CXMLWriter::CXMLWriter(std::shared_ptr< CDataSink > sink) {
	DImplementation = std::make_unique<SImplementation>(sink);
}

CXMLWriter::~CXMLWriter() {

}

bool CXMLWriter::Flush() {
	return DImplementation->Flush();
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
	return DImplementation->WriteEntity(entity);
}