//-----------------------------------------------------------------------------
#ifndef SINGLETON_H
#define SINGLETON_H
//-----------------------------------------------------------------------------

template <typename T>
class Singleton
{
  public:
    static T &Instance()
    {
      static T _instance;
      return _instance;
    }

  private:
    Singleton();
    ~Singleton();
    Singleton( Singleton const & );
    Singleton &operator=( Singleton const & );
};

#endif
