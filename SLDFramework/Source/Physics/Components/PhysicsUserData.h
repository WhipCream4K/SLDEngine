
#ifndef SLDFRAMEWORK_PHYSICSUSERDATA_H
#define SLDFRAMEWORK_PHYSICSUSERDATA_H


namespace SLD
{
	struct PhysicsUserData
	{
		b2BodyDef bodyDef{};
		b2FixtureDef fixtureDef{};
		SharedPtr<b2Shape> shape{};

		bool operator==(const PhysicsUserData& other) const
		{
			return bodyDef.userData.pointer == other.bodyDef.userData.pointer;
		}
	};
}


#endif
