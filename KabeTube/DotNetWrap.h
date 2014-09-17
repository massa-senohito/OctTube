
template<typename T>
class DotnetList
{
#ifdef _MANAGED
  List<T> data;
#else
  typedef std::vector<T> TS;
  //typedef std::vector<T>::iterator TSI;
  //template<typename T>
#endif
public:
  DotnetList<T>();
  TS* Data;
  ~DotnetList<T>();
  void Add(T);
  //TSI First();
  //TSI Last();
  void Clear();
  int Count;
  T operator[](int i);
};

template<typename T>
DotnetList<T>::DotnetList() :Count(0)
{
  Data = new TS();
}

template<typename T>
DotnetList<T>::~DotnetList(){
  Data->clear();
  delete Data;
}

template<typename T>
void DotnetList<T>::Add(T item){
  ++Count;
  Data->push_back(item);
}
template<typename T>
void DotnetList<T>::Clear(){
  Data->clear();
}
//template<typename T>
//TSI
//  DotnetList<T>::First()
//{
//  return data->begin();
//}
//template<typename T>
//TSI
//  DotnetList<T>::Last()
//{
//  return data->end();
//}

template<typename T>
T DotnetList<T>::operator[](int i)
{
  return Data[i];
}
#ifdef _MANAGED
#else
#endif