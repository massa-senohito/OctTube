#include "stdafx.h"
#include "GameAlgolyzm.h"
#include "PhysicSystem.h"
#include "Renderer.h"
#include "Assets.h"
#define scast static_cast
uint svgVLength;
Renderer* rend;
//焼けてる感じを出す
  //フィルターフラグでやればあるいは？しかし命中とらないことに変わりはない
  //気体に近いパラメータを与え、風は見えないパーティクルを出すことで表現
//ソケット通じてデバッガを作る＞ほぼ終了
//ステージクリア時にアニメを止める
//glRectで簡易描画,シンプルドローどこに
//蒸発してもその場にエフェクトとセンサーを作る
//団扇のためにセンサーを作る
  //センサーに接触判定が出ない、worldクエリしますか
//stbで画像ロードエフェクト
//どうしてもテクスチャデバッグができない
  //assimp使う　レガシーなサンプルかなにかで
  //別のデバッガ
//ステージ2以降のための整備
//リセット対応

//type Game =
//  let loop =
//    let poss = enes | >Seq.map GetPos
//    for i in enes->
//      let p = i.Pos
//    if debug.RenderSimple then i.Draw else drawRect p
//  let OnClear
//    setBackScreen
//    enes|>iter(fun e->e.NoAnim)
//    pla |>iter(fun p->p.Nodeath;p.Smoke|>)
//    state=printResult
//  
//  let OnBubbleHit pos
//    effect
//    createBlowOut pos
//  let
//    let edata=recast(fix->udata)
//    edata->DamagedYou(bubble)
//    match state with
//      |Stage1Clear->if oneShot
uint getSvgVLen(){
  return svgVLength;
}
#ifndef _MANAGED
namespace File{
  stringArray* ReadAllLines(String name){
		FILE* p;
    const int maxCharInLine = 256;
    auto e=fopen_s(&p, name, "r");
    if (e == 0){
      char* line = new char[maxCharInLine];
      auto lines = new stringArray;
      while (fgets(line, maxCharInLine, p))
      {
        lines->push_back(line);
      }
      fclose(p);
      return lines;
    }
    else if (e==ENOENT){
      std::cout << "ファイル:"<<name << " がありません" << std::endl;
      return nullptr;
    }
    return nullptr;
  }
}
namespace Convert{
	double ToSingle(String str){
    return atof(str);
	}
}
PPhysicSystem sys;//managedなので外に置けない
#endif

float toRad(float ang){return ang/180*3.141592f;}
float toDeg(float deg){return deg*180/3.141592f;}
void drawClothHair(float angle){
  float radius=6;
  
  float vx=radius*cos(toRad(angle));
  float vy=radius*sin(toRad(angle));
  glRectf(vx-2,vy+2,vx+2,vy-2);
}
namespace Assets{
  Points squid;
  uint*  squidElem;//テンプレートメタプログラミングで最適化が望めるが、ここではやらんでおく
  int    squidLen;

  Points getSquidPoints(){
    return ( squid);
  }
  uint* getSquidElem(){
    return squidElem;
  }
  int squidPLen(){
    return squidLen;
  }
  Points wall;
  uint*  wallElem;
  int    wallLen;

  Points getWallP(){
    return ( wall);
  }
  uint* getWallElem(){
    return wallElem;
  }
  int wallPLen(){
    return wallLen;
  }

  //後ほどクラスに移動する
  Points tako;
  uint*  takoElem;
  int    takoLen;
  Points getTakoPoints(){
    return tako;
  }
  uint* getTakoElem(){
    return takoElem;
  }
  int getTakoLen(){
    return takoLen;
  }
}
/*
let tt t=
thre * (one-t)* (one-t)* t
let t3 t=
thre*t*t*(one-t)
let ttt t=t*t*t
let bejx xx xxx t=
(tt t) * xx * (t3 t) * xxx+ttt(t)
*/
const float onef = 1.0f;
PEnes ens;
PTys  tys;
float bej(float t){
  return 3.0f * (onef - t)*(onef - t)*t *
    onef * 3.0f * t * t *(onef - t)  * onef + t*t*t;
}
//typedef std::basic_istringstream<char> istringstream;
//using vector<T>=std::vector<T>;
//パースしたデータはランダムアクセスで拾うのでvectorでいいや
//http://shnya.jp/blog/?p=195
std::vector<std::string> Split(std::string str, char delim){
  std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
  while(getline(iss, tmp, delim)) res.push_back(tmp);
  return res;
}
float strTofloat(std::string str){
    return scast<float>(Convert::ToSingle(str.c_str()));
}
void GameAlgolyzm::makeMonster(stringArray coes )
{
  //縦に動く,x10y12に初期値から加速度20で移動,（こげやすさ
  auto name = coes[0];
  auto pat  = coes[1];
  auto x    = strTofloat( coes[2]);
  auto y    = strTofloat( coes[3]);
  if(pat=="vmove"){
    
  }
  //-16,11でとばす
  auto enemy = 
    name == "squid"      ? addEnemy(-32,0,1,EnemyKind::Squid) :
    name == "dragonbody" ? addEnemy(-32,0,1,EnemyKind::Squid) :
                           addEnemy(-32,0,1,EnemyKind::Squid);
  
  enemy->Impulse(V2(6,11));

  //enemy->SetAssets(Squid); //
  //enemy->SetPoints(ps,Assets::squidPLen());
}
//#define Path SvgFs::Loader::Path 

Points svgRead(String name){
  /*  M始点 C 点 前の点　コントロールポイント 次の点
  M 93.00,37.00
  C 93.00,37.00 80.00,65.00 59.00,72.00
  59.00,72.00 84.00,109.00 71.00,125.00
  71.00,125.00 87.00,152.00 114.00,122.00
  102.00,97.00 130.00,74.00 130.00,74.00
  112.00,71.00 93.00,37.00 93.00,37.00 Z
  M 79.00,143.00
  C 79.00,143.00 66.00,175.00 66.00,175.00M 110.00,141.00
  C 110.00,141.00 118.00,179.00 118.00,179.00M 94.00,146.00
  C 94.00,146.00 94.00,181.00 94.00,181.00
  */
  //  auto fi=Loader::openGimpPath("squid.svg");
  //  auto anim=Loader::anim(fi,0);//たしかにfs側でbyte[][]吐き出せば静的に焼きこんだアニメーション作れる
  //  auto ps=Loader::pathToPointsLis(anim,0.2f);
#ifdef _MANAGED
  auto ps =
    gcnew array<float>(0);
#else
  FILE* p;
  auto e = fopen_s(&p, name, "rb");
  if (e == 0){
    int flamenum = 0;
    svgVLength= 0;
    int count = 0;
    size_t t = 1;
    fread(&flamenum, sizeof(int), (size_t)1, p);
    fread(&svgVLength, sizeof(int), (size_t)1, p);
    //freadするために1次元
    auto tmp = new float[(flamenum*svgVLength)];
    //auto tes = new float[flamenum][442];
    while (t != 0){
      tmp; //->push(new float[svgVLength]);
      t = fread(
        &tmp[count*svgVLength]
        //tes[count]
        , sizeof(float), (size_t)svgVLength, p);
      ++count;
      //t=0;
    }
  fclose(p);
  return tmp;
  }

#endif
  
  std::cout << name << " can't read" << std::endl;
  int c=1;//ps->Length;
  for (int i = 0; i < c; i++)
  {
    //ps[i]->
  }
  return nullptr;
}
void GameAlgolyzm::fileRead(String filename,PPhysicSystem sys)
{
  auto fi=File::ReadAllLines(filename);
#ifdef _MANAGED
  int c=fi->Length;
#else
  int c = fi->size();
#endif
  for (int i = 0; i < c; i++)
  {
#ifdef _MANAGED
   auto coefar=fi[i]->Split(',');
#else
    auto coefar = Split(fi->at(i), ',');
#endif
   std::string name=coefar[0]; 
   if(name=="Cube"){
     auto x = strTofloat(coefar[1]);
     auto y = strTofloat(coefar[2]);
     auto z = strTofloat(coefar[3]);
     auto sx= strTofloat(coefar[4]);
     auto sy= strTofloat(coefar[5]);
     auto sz= strTofloat(coefar[6]);
     sys->addFence(x,y,sx,sy);
     continue;
   }
   if(name=="blow"){
     auto x  = strTofloat(coefar[1]);
     auto y  = strTofloat(coefar[2]);
     auto dx = strTofloat(coefar[3]);
     auto dy = strTofloat(coefar[4]);
     auto rad= strTofloat(coefar[5]);
     Blow(
       V2(x, y), V2(dx,dy), rad
     );
     continue;
   }
   //全部違うならモンスター
   makeMonster(coefar);
  }
}

///最後に撃った時のフレーム
int lastFiredFrame=int(0);
float angle=float(90);
#define pos 0.5f,0.4f
float posx = 29; float posy = -18.5f;
void playerControl(int time){
  //
  int movableF = 2;
  float power  = 18;
  glTranslatef( posx + 30 , posy - 60 , 0.0f );
  glRotatef(180, 1, 0, 0);
  glBegin(GL_LINES);

  static bool state = false;
  const int animLen = 38;
  if (time % animLen == 0) state = !state;
  int anim = state ? time % animLen : animLen - time % animLen;
  renderVertice( Assets::tako , Assets::takoLen , anim/2 );  //ここと敵部分
  //enem->Update()
  glEnd();
  //たこの触手をキーで動かせるが、足はベジエカーブを移動させればそれなりの見栄えになるみたい
  //吹き飛ばせるように
  //drawClothHair(angle);
  glLoadIdentity();
  
  //std::cout << angle << std::endl;
  if(Key::isAPushed()){//フレームじゃなくてタイマー
    if(time-lastFiredFrame>10 
      //&& !AllEnemyFired()
      ){
      float vx=cos(toRad(angle));
      float vy=sin(toRad(angle));
      makeSinglePar
        ( posx , posy , vx*power , vy*power );
      makeSinglePar
        ( posx-3 , posy-3 , vx*power , vy*power );
      makeSinglePar
        ( posx-2 , posy-5 , vx*power , vy*power );
      lastFiredFrame=time;
    }
  }
  //auto s = 0.5f;
  if(Key::isLeftPushed()){
    if(time%movableF==0){
      angle+=0.5f;
      //posx -= 0.5f;
    }
  }
  if(Key::isRightPushed()){
    if(time%movableF==0){
      angle-=0.5f;
      //posx += 0.5f;
    }
  }
  if(Key::isUpPushed()){
    if(time%movableF==0){
      posy += 0.5f;
    }
  }
  if(Key::isDownPushed()){
    if(time%movableF==0){
      posy -= 0.5f;
    }
  }
  //ここからはデバッグ用
  if (Key::isDPushed()){
    ens->ForEach([](PEnemy e){e->SetHp(1); });
  }
}
//float scalex = 0, scaley = -98.5;
//timeは17ミリ秒ごとにカウント
//void drawFish(
void onRenderFrame(int time){
  //if(c%50==0){makeParticle(pos);}
  playerControl(time);
  sys->Step();
#ifdef _DEBUG //todo デバッグ用の壁描画 
  if (ens->Count != 0){
    auto& data = **ens->Data->begin();
    //AIの行動範囲
    sys->DrawAABB(data.GetActBox());
  }
#endif
  glScalef(0.17f, 0.2f,1.f);
  glRotatef(180, 1, 0, 0);
  glTranslatef(-263.5f,-98.5f, 0);
  glBegin(GL_LINES);
#ifdef _DEBUG //todo デバッグ用の壁描画 
  renderVertice(Assets::wall, Assets::wallLen,0);
#else
  renderVertice(Assets::wall, Assets::wallLen,0);
#endif
  glEnd();
  glLoadIdentity();
}
GameAlgolyzm::GameAlgolyzm(stringArray args)
:sys(gcnew PhysicSystem)
 , stage(Stages::Stage1)
 , isClear(0)
 , clearStatus(*(gcnew DotnetList< ClearStatus*>()))
 , message(new char[128])
 , currentStagePath(*(new std::string()))
  //:ens(gcnew Enes),
  //tys(gcnew Tys)
{
  ens = gcnew Enes;
  tys = gcnew Tys ;
  auto path    = args[0];
  auto lastBel = path.find_last_of('\\');
  path         = path.substr(0,lastBel+1);
  path         += "assets\\";
  currentStagePath = path;
  Path::setPass(path);
  Assets::tako     = svgRead((path+"takoallFlame").data());
  Assets::takoLen  = svgVLength;
  Assets::takoElem = gcnew uint[svgVLength];
  for (size_t i = 0; i < svgVLength; i++)
  {
    Assets::takoElem[i] = i;
  }
  //todo ステージ情報、壁をAssets内にもっていって、適宜disposeできるように
  Assets::wall    = svgRead((path + "wallallflame").data());
  Assets::wallLen = svgVLength;

  int len = 0; //(args->Length);
  //glewInit呼ばないといけないので
  GLUT_INITs(len ,nullptr);
  //sys->MakeParticle(pos);
  //char ** arg= strMap(args);//opentkだと？
  ///drawable,steppable作るべき

  this->SetStage(stage);
  rend=new Renderer([&](int count){
    onRenderFrame(count);
    Step();
  }
  , [&](){delete this; }
  );
  //auto g=new GUIUtil();
  //Trueになる
  //Console::WriteLine(arg[0]);
  //std::cout << (arg[0])<< std::endl;;
  rend->renderPolygon(nullptr,0);
  glutMainLoop();
}
std::string tos(int i){ return std::to_string(i)+'\n'; }
std::string leg(int i){ return "leg"+tos(i)+": "; }
///クリア評価
ClearStatus* GameAlgolyzm::HowAllMeatFired(int* ps)
{
  if (stage == Stage1)
  {
    auto asiPoint = ps[1] + ps[2] + ps[3];
    auto clrstr = std::string("squid: ") + tos(ps[0])
      + leg(0) + tos(ps[1])
      + leg(1) + tos(ps[2])
      + leg(2) + tos(ps[3]);
    if (asiPoint>20){ clrstr += "\ntastes good."; }
    strcpy(( message), clrstr.data());
    return (new ClearStatus( asiPoint > 20, asiPoint + ps[0]));
  }
  return(new ClearStatus(false, 0));
  //return 1;
}

bool GameAlgolyzm::AllEnemyFired(){
  if (isResultScreen())return true;
  if (isClear )return true;
  auto count = ens->Count;
  auto& e = (*ens);
  for (int i = 0; i < count; i++)
  {
    if (!e[i]->IsAllMeatFired())return false;
  }
  return true;
}
Typhoon* GameAlgolyzm::Blow(V2 p, V2 dir, float rad){
  auto typ = new Typhoon(sys->GetWorld(),p,dir,rad);
  tys->Add(typ);
  return typ;
}
PEnemy GameAlgolyzm::addEnemy(f32 x,f32 y,f32 rad,EnemyKind ek){
  //onClearStageはシーン的なもの、これは
  auto e=gcnew Enemy(sys->GetWorld(),V2(x,y),rad,ek);
  ens->Add(e);
  return e;
}
void GameAlgolyzm::SetStage(Stages s){
  auto si=scast<int>(s);
  auto& csv = currentStagePath + "coe" +std::to_string(si)+ ".csv";
  stage = s;
  //deleteEnemyBody
  
  sys->delFences();
  fileRead(csv.data(),sys);
}
Stages GameAlgolyzm::onClearStage(Stages s)
{
  auto data = ens->Data;
  auto sc=ens->Accumlate([](int acc, PEnemy e){return acc + 2; });
  auto ps = (data->at(0)->GetScore());  //Map(*data, gain);
  clearStatus.Add(HowAllMeatFired(ps));
  if (isResultScreen())
  {
    return s;
  }
  //%2で割り切れたらボタン入力待ち
  else{
    return scast<Stages>(stage+1);
  }
}
void GameAlgolyzm::PrintResult(){
  glRasterPos2f(10, 10);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
    reinterpret_cast<const unsigned char*>(message));
}
bool GameAlgolyzm::isResultScreen(){
  return stage % 2;
}
void GameAlgolyzm::Step(){

  auto stopping = sys->GetStopping();
  bool fired=AllEnemyFired();
  if (!fired){
#ifdef _MANAGED
  for each (PEnemy var in ens)
  {
    var->Update();
    //列挙中に消えるとめんどくさいのでmap<,bool>
    //if(var)
  }
#else
    ens->ForEach(
    //std::for_each(data->begin(), data->end(),
      [&stopping](PEnemy i){ i->Update(stopping == 0); });
    step();
#ifdef _DEBUG
    //sys->DrawDebug();
#endif
#endif
  }
  else{
    //結果画面表示,自分自身もsetstageして、ボタン入力待ちに
    //if (Key::getOneShotPushed()[Key::A])
    if (!isResultScreen()&&!isClear)
    {
      //クリア後1度だけ
      stage = onClearStage(stage);
      rend->SetStage(stage);
      ens->Clear();
      isClear = true;
    }
    else{
      //ctrlがoneshotしたら次のステージへ
      if (Key::getOneShotPushed()[Key::A]){
        SetStage(scast<Stages>(stage+1));
        ens->ForEach([](PEnemy e){
          //e->Kind++;
          //e->UnLoad(); するResetStage
          //e->SetProfile(EnemyKind::Dragon);
        });
        rend->SetStage(stage);
        isClear = false;
      }
    }
    PrintResult();
  }
  //jointDraw();
}
void GameAlgolyzm::
  Render()
{
  int length=0;//w.ParCount
  //auto pb=w.PartiPosBuf()
  for (int i = 0; i < length; i++)
  {
    //setMask(i%3*2+1

  }
}
GameAlgolyzm::~GameAlgolyzm(void)
{
  //delete時にclear走ってるので
  SAFE_DELETE(ens);
  SAFE_DELETE(tys);
  clearStatus.Clear();
  delete(&(clearStatus));
  SAFE_DELETE(rend);
  SAFE_DELETE(message);
  DA ( Assets::squid);
  DA ( Assets::squidElem);
  DA ( Assets::tako);
  DA ( Assets::takoElem);
  DA ( Assets::wall);
  DA ( Assets::wallElem);
  SAFE_DELETE( sys );
}
