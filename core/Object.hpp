#ifndef OBJECT_H
#define OBJECT_H


class Object
{

public:
	Object();
	virtual ~Object() {}


	int				getRefCount() const { return m_refCount; }
	void			retain() { m_refCount++; }
	void			release() { m_refCount--; if( !m_refCount ) { delete this; }  }

private:
	int				m_refCount;

};

#endif // OBJECT_H