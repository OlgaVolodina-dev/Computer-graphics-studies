#include "UBO.h"

std::size_t IUBOListener::GetUBOSize() { return 0; }
void IUBOListener::SetData(std::size_t offset) {}

UBO::UBO()
{
	glGenBuffers(1, &buffer_);
}

void UBO::Setup()
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
	for (auto& listener : listeners_)
	{
		size_ += listener->GetUBOSize();
	}
	glBufferData(GL_UNIFORM_BUFFER, size_, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	programIndex_ = 0U;
	bindingIndex_ = 0U;
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex_,
		buffer_, 0, size_);
}

void UBO::RegisterListener(IUBOListener& listener)
{
	listeners_.push_back(&listener);
}

void UBO::UpdateUBO()
{

	glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex_,
		buffer_, 0, size_);
	std::size_t offset = 0;
	for (auto& listener : listeners_) {
		listener->SetData(offset);
		offset += listener->GetUBOSize();
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer_);

}