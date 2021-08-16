#include "Galagas.h"

void Galagas::OnCreate(const SharedPtr<SLD::GameObject>& gameObject)
{
	m_Instance = gameObject;

	
}

const SharedPtr<SLD::GameObject>& Galagas::GetGameObject() const
{
	return m_Instance;
}
