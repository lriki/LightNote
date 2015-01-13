// Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


#ifndef LNOTE_FUNCLIB_TEST

#include "../../Source/Core/Graphics/Common/Bitmap.h"
#include "../../Source/Core/Effect/Effekseer/EffekseerEffectEngine.h"
#include "../../Source/Core/Effect/Effekseer/EffekseerEffectCore.h"
#include "../../Source/Core/Effect/Effekseer/EffekseerEffectInstance.h"

#include "../../Source/Dependencies/Effekseer/Effekseer.h"
#include "../../Source/Dependencies/Effekseer/EffekseerRendererDX9/EffekseerRendererDX9.h"

#include "../../Source/Core/Graphics/Common/Model/MMDModelLoader.h"
#include "../../Source/Core/Graphics/Common/Model/PMXLoader.h"






//-------------------------------------------------------------
void Test10_C_API()
{
	//LConfig_SetWindowResizeEnabled(LN_FALSE);
	LConfig_SetToggleFullscreenEnabled(LN_FALSE);
	LConfig_RegisterArchive( "../../Samples/Media/SampleArchive.lna", "test" );

	LNote_Init();

	
	LInput_AttachVirtualButton( 0, LN_BUTTON_A, 0 );

#if 0 // ファイルテスト
	char buf[1000] = {0};
	FILE* fp = fopen( "../../Samples/Media/SampleArchiveData/TextFile.txt", "r" );
	_p(FileIO::File::getFileSize(fp));
	_p(fread( buf, 1, FileIO::File::getFileSize(fp), fp ));
	_p(ftell(fp));
	printf( "[[%s]]",  (char*)buf );


	lnHandle texture1 = LTexture_Create( 640, 480 );
	lnHandle sprite1  = LSprite_Create( texture1 );

	lnRect rect1 = { 0, 0, 640, 480 };
	LTexture_DrawText( texture1, buf, &rect1 );




	lnHandle file1 = LInFile_Create( "../../Samples/Media/SampleArchive/TextFile.txt", LN_FILEOPENMODE_TEXT );
	int size = LInFile_GetSize( file1 );
	lnByte* data = new lnByte[size];
	int readSize = LInFile_Read( file1, data, size );
	
	_p(size);
	_p(readSize);
	printf( "[[%s]]",  (char*)data );

	delete data;
#endif


	lnHandle texture1 = LTexture_CreateFromFile( "../LightNote.UnitTest/Data/20140309.png" );
	lnColor c1 = { 1, 0, 0, 1 };
	//LTexture_Clear( texture1, &c1 );
	lnHandle sprite1 = LSprite_Create3D( texture1 );
	LSprite_SetSize( sprite1, 1, 1 );
	//LNode_SetPosition( sprite1, -3, 0, 0 );
	LNode_SetPlacementType( sprite1, LN_PLACEMENTTYPE_BILLBOARD );
	//LSprite_SetTexture( sprite1, texture1 );

	
	lnHandle sprite3 = LSprite_Create( texture1 );


	lnHandle sprite2 = LSprite_Create3D( texture1 );
	LSprite_SetSize( sprite2, 1, 1 );
	LNode_SetPlacementType( sprite2, LN_PLACEMENTTYPE_BILLBOARD );
	LNode_AddChild( sprite1, sprite2 );
	LNode_SetPosition( sprite2, 3, 0, 0 );


	lnHandle camera = LCamera_GetDefault3DCamera();
	LCamera_SetPosition( camera, 10, 10, -10 );



	lnHandle toneFilter = LToneFilter_Create();
	lnTone tone1 = { 0.0, -0.3, -0.7, 0.8 };//{0,0,0,0};//
	LToneFilter_SetTone( toneFilter, &tone1, 0 );
	//LViewPane_AttachImageFilter( LViewPane_GetDefaultViewPane(), toneFilter );


	lnHandle maskTex = LTexture_CreateFromFile( "D:/Proj/lightnote/LightNote/Samples/Media/star_b.png" );


	lnHandle transitionFilter =  LTransitionFilter_Create();
	//LTransitionImageFilter_Freeze( transitionFilter );
	//LTransitionImageFilter_Transition( transitionFilter, 3, maskTex, 40 );
	//LViewPane_AttachImageFilter( LViewPane_GetDefaultViewPane(), transitionFilter );


	lnHandle blurFilter =  LBlurFilter_Create();
	LViewPane_AttachViewFilter(LViewPane_GetDefaultViewPane(), blurFilter, 0);
	//LViewPane_AttachImageFilter( LViewPane_GetDefaultViewPane(), blurFilter, 10 );
	//LViewPane_DetachImageFilter( LViewPane_GetDefaultViewPane(), blurFilter );

	lnColor blue = { 0, 0, 1, 1 };
	LViewPane_SetBackgroundColor( LViewPane_GetDefaultViewPane(), &blue );

	//_p(D3DXVec2Length(NULL));

	//new int(5);

	float angle = 0.0;

	do
	{
		LNode_SetAngle( sprite1, angle, 0, 0 );
		LNode_SetAngle( sprite1, 0, angle, 0 );
		LNode_SetAngle( sprite1, 0, 0, angle );

		angle -= 0.01;

		if ( LInput_IsTrigger( 0, LN_BUTTON_A ) )
		{
			LTransitionFilter_Freeze( transitionFilter );
			LTransitionFilter_Transition( transitionFilter, 3, maskTex, 40 );

		}

		if ( LInput_IsTrigger( 0, LN_BUTTON_B ) )
		{
			lnVector3 center = { 1, 0, 0 };
			LBlurFilter_Blur( blurFilter, 1.5, 0.5, 1.02, &center, NULL );

		}

	} while ( LNote_Update() );

	LNote_End();
}



//-------------------------------------------------------------
void Test09_Mesh()
{
	Core::ConfigData data;
	data.EnableCRTSetDebugFlags = true;
	data.EnableShowFPS = true;
	data.BackbufferSize.set( 640, 480 );
	data.EnablePhysicsThread = true;
	Core::Manager* fw = LN_NEW Core::Manager();

	try
	{
		fw->initialize( data );
		fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setCameraOperator(
			Graphics::CylinderMouseMoveCameraOperator::create());

#if 0
#if 1
		//fw->getFileManager()->moveCurrentDirectory("D:/MMD/モデル/Lat式ミクVer2.3");
		fw->getFileManager()->moveCurrentDirectory("D:/MMD/モデル/536式電ver1.10");
		Graphics::MMDModelLoader mmdModelLoader;
		//LRefPtr<FileIO::InFile> file1( fw->getFileManager()->createInFile("D:/MMD/モデル/Lat式ミクVer2.3/Lat式ミクVer2.3_Normal.pmd") );
		LRefPtr<FileIO::InFile> file1( fw->getFileManager()->createInFile("D:/MMD/モデル/536式電ver1.10/536式電ver1.10.pmx") );
		//Graphics::ModelCore2* mc2 = mmdModelLoader.load( fw->getGraphicsManager()->getModelManager(), file1, false );
		
		Graphics::PMXLoader pmxLoader;
		Graphics::ModelCore2* mc2 = pmxLoader.load( fw->getGraphicsManager()->getModelManager(), file1, false );
		LN_SAFE_RELEASE(mc2);
		fw->getFileManager()->returnCurrentDirectory();
#endif




		LRefPtr<Scene::Mesh> mesh( LN_NEW Scene::Mesh( fw->getSceneManager()->getDefaultSceneGraph() ) );
		mesh->create( "D:/MMD/モデル/ぜかまし1.2/島風.pmd" );
		//mesh->create( "D:/MMD/モデル/Tda式初音ミク・アペンドVer1.00/Tda式初音ミク・アペンド_Ver1.00.pmx" );

		LRefPtr<Scene::Sprite3D> sp1( LN_NEW Scene::Sprite3D( fw->getSceneManager()->getDefaultSceneGraph() ) ); 
		sp1->create( 10, 10 );
		//getDepthBuffer

		fw->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->activate();
		sp1->setTexture(
				fw->getGraphicsManager()->getGraphicsDevice()->getRenderer()->getDepthBuffer() );

		// 背景球
		LRefPtr<Scene::Mesh> mesh2( LN_NEW Scene::Mesh( fw->getSceneManager()->getDefaultSceneGraph() ) );
			mesh2->create( "D:/MMD/ステージ/X/skydome.x" );
		mesh2->setScale( LVector3( 100, 100, 100 ) );

		LRefPtr<Scene::ToneImageFilter> filter1( LN_NEW Scene::ToneImageFilter() );
		filter1->create( fw->getSceneManager() );
		fw->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->attachImageFilter( filter1 );

		//filter1->setTone( LTone( 1, 1, 1, 0 ), 2.0 );
#endif
			

		LRefPtr<Scene::Model> model1( LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() ) );
		//LRefPtr<Scene::Mesh> model1( LN_NEW Scene::Mesh( fw->getSceneManager()->getDefaultSceneGraph() ) );
		//model1->create( "D:/MMD/モデル/536式電ver1.10/536式電ver1.10.pmx" );
		model1->create( "D:/MMD/モデル/ぜかまし1.2/島風.pmd" );
		//model1->create( "D:/MMD/モデル/Appearance Miku/Appearance Miku_BDEF.pmx" );
		//model1->create( "D:/MMD/モデル/AliceV033/アリスV033.pmd" );
		//model1->create("D:/MMD/モデル/Tda式初音ミク・アペンドVer1.00/Tda式初音ミク・アペンド_Ver1.00.pmx");
		//model1->create("D:/MMD/モデル/Appearance Miku/Appearance Miku_BDEF.pmx" );

		LRefPtr<FileIO::InFile> motionFile1( fw->getFileManager()->createInFile("D:/MMD/モーション/Zigg-Zagg/ZZ-MikuV2.vmd") );
		LRefPtr<Animation::AnimationClip> motion( fw->getGraphicsManager()->getModelManager()->createMotion2( motionFile1 ) ); 
		

		model1->getModelSource()->getAnimationController()->addAnimationClip( motion );


		
		//LRefPtr<Scene::SceneShader> shader1( fw->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader(
		//	"MMMTest.lnfx" ) );
		//model1->setShader( shader1 );
#if 0
		const int ModelCount = 4;
		LRefPtr<Scene::Model> models[ModelCount];
		models[0].attach( LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() ) );
		models[0]->create( "D:/MMD/モデル/Tda式初音ミク・アペンドVer1.00/Tda式初音ミク・アペンド_Ver1.00.pmx" );
		models[0]->setPosition( LVector3( 10, 0, -10 ) );
		models[0]->getModelSource()->getAnimationController()->addAnimationClip( motion );

		models[1].attach( LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() ) );
		models[1]->create( "D:/MMD/モデル/536式電ver1.10/536式電ver1.10.pmx" );
		models[1]->setPosition( LVector3( 10, 0, 10 ) );
		models[1]->getModelSource()->getAnimationController()->addAnimationClip( motion );

		models[2].attach( LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() ) );
		models[2]->create( "D:/MMD/モデル/ぜかまし1.2/島風.pmd" );
		models[2]->setPosition( LVector3( -10, 0, -10 ) );
		models[2]->getModelSource()->getAnimationController()->addAnimationClip( motion );
		
		models[3].attach( LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() ) );
		models[3]->create( "D:/MMD/モデル/AliceV033/アリスV033.pmd" );
		models[3]->setPosition( LVector3( -10, 0, 10 ) );
		models[3]->getModelSource()->getAnimationController()->addAnimationClip( motion );
#endif

		
		fw->resetDelayTime();
		LVector3 pos;
		LVector3 vel;
		do
		{
			model1->getModelSource()->getAnimationController()->advanceTime( 0.016 * 0.5 );

			if (fw->getInputManager()->getGameController(0)->isOnTrigger(Input::LN_BUTTON_A))
			{
				vel.y = 1.0;
			}
			pos.y += vel.y;
			vel.y -= 0.1;
			if (pos.y < 0)
			{
				pos.y = 0;
				vel.y = 0;
			}

			model1->setPosition(pos);

#if 0
			for ( int i = 0; i < ModelCount; ++i )
			{
				LVector3 tp;
				tp = models[i]->getPosition();
				tp.y = pos.y;
				models[i]->setPosition(tp);
				models[i]->getModelSource()->getAnimationController()->advanceTime( 0.016 * 0.5 );
			}
#endif

			//mesh2->setPosition(
			//	fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->getPosition() );



				//fw->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->getDepthBuffer());


			//fw->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->present();
			//fw->getGraphicsManager()->getGraphicsDevice()->testDeviceLost();
		} while ( fw->update() );
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}


//-------------------------------------------------------------
void Test08_Physics()
{
	//Vector p = {0, 0, 1};
	//Vector v = {0, 0, 1};
	//RotateByQuaternion(v, 

	//LVector3 axis(0, 0, 1);	// 反時計回り
	//float rad = 0.01;

	//LQuaternion qB(axis, rad);
	//LQuaternion qA;
	//LQuaternion::conjugate( &qA, qB );

	//LQuaternion qPos( 1, 0, 0, 0 );
	//LQuaternion qAns = qB * qPos;
	//qAns = qAns * qA;

	//qPos = (qB * qPos) * qA;


	Core::ConfigData data;
	data.EnableCRTSetDebugFlags = true;
	data.BackbufferSize.set( 640, 480 );
	Core::Manager* fw = LN_NEW Core::Manager();


	try
	{
		fw->initialize( data );

		fw->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->setBackgroundColor(LColor(0.5, 0.5, 0.5));

		fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setCameraOperator(
			Graphics::CylinderMouseMoveCameraOperator::create());
		 
		LRefPtr<Graphics::ITexture> texture1(
			fw->getGraphicsManager()->getGraphicsDevice()->createTexture( 
			"D:/GameProjects/Ota-GamePGs/Volkoff/Volkoff_vs2012/Volkoff_vs2012/Data/Graphics/Character/Char_BareHands.png" ));

		LRefPtr<Scene::BatchSprite3D> sprite1( LN_NEW Scene::BatchSprite3D( fw->getSceneManager()->getDefaultSceneGraph() ) );
		sprite1->create( 10, 10 );
		sprite1->setTexture( texture1 );
		sprite1->setOpacity( 0.9);


		LRefPtr<Physics::Plane> plane1( LN_NEW Physics::Plane() );
		plane1->initialize( fw->getPhysicsManager() );
		plane1->setUserData( (void*)100 );


		Physics::RigidBody::ConfigData box1Data;
		box1Data.Mass = 0.001f;
		LRefPtr<Physics::Box> box1( LN_NEW Physics::Box() );
		box1->create( fw->getPhysicsManager(), LVector3( 1, 1, 1 ), box1Data );
		LMatrix m1;
		m1.translation( 0, 10, 0 );
		box1->setWorldMatrix( m1 );

		LRefPtr<Physics::CollisionBox> colBox1( LN_NEW Physics::CollisionBox() );
		colBox1->create( fw->getPhysicsManager(), LVector3( 1, 1, 1 ), 0xffff, 0xffff );


		LRefPtr<Physics::CharacterObject> chara1( LN_NEW Physics::CharacterObject() );
		chara1->createCapsule( fw->getPhysicsManager(), 0.3, 1 );

		LRefPtr<Physics::CharacterObject> chara2( LN_NEW Physics::CharacterObject() );
		chara2->createCapsule( fw->getPhysicsManager(), 0.3, 1 );


		do
		{
			//printf("%f\n", box1->getWorldMatrix().getPosition().y);

			if ( box1->getContactCount() > 0 )
			{
				//printf( "data:%d\n", box1->getContact(0).TargetBody->getUserData() );
			}

			if ( colBox1->getContactCount() > 0 )
			{
				//printf( "data colBox1:%d\n", colBox1->getContact(0).TargetBody->getUserData() );
			}

			//printf( "data chara1:%d\n",  chara1->getContactCount());
			if ( chara1->getContactCount() > 0 )
			{
				//chara1->getContact(0).Normal.dump();
				//printf( "data chara1:%d\n", );
				if ( chara1->getContactCount() > 1 )
				{
					//chara1->getContact(1).Normal.dump();
				}
			}

			Input::GameController* input = fw->getInputManager()->getGameController(0);

			float jump = 0;
			if (input->isOnTrigger( Input::LN_BUTTON_A ) )
			{
				jump = 10;
			}

			const LVector2& v = input->getAxisState(0);
			chara1->move(LVector3(v.x, jump, -v.y));
			//v.dump();

			
		}
		while ( fw->update() );
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void Test07_Font()
{
	Core::ConfigData data;
	data.EnableCRTSetDebugFlags = true;
	data.BackbufferSize.set( 640, 480 );
	Core::Manager* fw = LN_NEW Core::Manager();

	try
	{
		fw->initialize( data );

		LRefPtr<Graphics::ITexture> texture1(
			fw->getGraphicsManager()->getGraphicsDevice()->createTexture( 640, 512, 1 ) );
		texture1->clear( LColor::WHITE );

		LRefPtr<Scene::Sprite> sprite1( LN_NEW Scene::Sprite( fw->getSceneManager()->getDefaultSceneGraph() ) );
		sprite1->create();
		sprite1->setTexture( texture1 );

		// フォントファイル登録
		fw->getGraphicsManager()->getFontManager()->addFontFile(
			"D:/Proj/TestMat/VLGothic/VL-PGothic-Regular.ttf" );

		// フォントファイル登録
		fw->getGraphicsManager()->getFontManager()->addFontFile(
			"D:/Proj/TestMat/VLGothic/VL-Gothic-Regular.ttf" );
		
		// フォントファイル登録
		//fw->getGraphicsManager()->getFontManager()->addFontFile(
		//	"D:/Proj/TestMat/Font/msgothic.ttc" );

		// フォントファイル登録
		fw->getGraphicsManager()->getFontManager()->addFontFile(
			"D:/Proj/TestMat/Font/meiryo.ttc" );


		LRefPtr<Graphics::IFont> fontVLGothic( fw->getGraphicsManager()->getFontManager()->createFont() );
		fontVLGothic->setName("VL Gothic");
		texture1->setFont( fontVLGothic );
		texture1->drawText( _T("ABA VL Gothic "), LRect( 8, 8, 512, 512 ) );

		LRefPtr<Graphics::IFont> fontVLPGothic( fw->getGraphicsManager()->getFontManager()->createFont() );
		fontVLPGothic->setName("VL Gothic");
		fontVLPGothic->setSize( 32 );
		fontVLPGothic->setItalic( true );
		fontVLPGothic->setAntiAlias( true );
		texture1->setFont( fontVLPGothic );
		texture1->drawText( _T("VL Gothic  : アンチエイリアス + 斜体"), LRect( 8, 8 + 100 * 1, 512, 512 ) );

		LRefPtr<Graphics::IFont> fontMSPGothic( fw->getGraphicsManager()->getFontManager()->createFont() );
		fontMSPGothic->setName("MS PGothic");
		fontMSPGothic->setBold( true );
		fontMSPGothic->setEdgeSize( 2 );
		fontMSPGothic->setEdgeColor( LColor(0.7, 0.9, 1.0) );
		texture1->setFont( fontMSPGothic );
		texture1->drawText( _T("MS PGothic : アウトライン"), LRect( 8, 8 + 100 * 2, 512, 512 ) );

		LRefPtr<Graphics::IFont> fontMeiryoUI( fw->getGraphicsManager()->getFontManager()->createFont() );
		fontMeiryoUI->setName("Meiryo UI");
		fontMeiryoUI->setSize( 40 );
		fontMeiryoUI->setEdgeSize( 2 );
		fontMeiryoUI->setEdgeColor( LColor(1.0, 0.7, 0.5) );
		fontMeiryoUI->setAntiAlias( true );
		texture1->setFont( fontMeiryoUI );
		texture1->drawText( _T("Meiryo UI : アンチ + アウトライン"), LRect( 8, 8 + 100 * 3, 512, 512 ) );

		LRefPtr<Graphics::IFont> fontSystem( fw->getGraphicsManager()->getFontManager()->createFont() );
		fontSystem->setName("МＳ Ｐゴシック");
		fontSystem->setSize( 40 );
		fontSystem->setAntiAlias( true );
		texture1->setFont( fontSystem );
		texture1->drawText( _T("システムフォント ：МＳ Ｐゴシック ij"), LRect( 8, 8 + 100 * 4, 512, 512 ) );

		LRect rc;
		fontVLGothic->getTextSize("VL Gothic", 1, &rc );
		rc.dump();


		//fontVLGothic->setName("MS PGothic");
		//
		//font1->setAntiAlias( true );
		//font1->setSize(32);
		//font1->setEdgeColor(LColor::BLUE);
		//font1->setEdgeSize(2);

#if 1
		sprite1->setVisible( false );


		Graphics::IGraphicsDevice* gd = fw->getGraphicsManager()->getGraphicsDevice();
		Scene::SceneGraph* sg = fw->getSceneManager()->getDefaultSceneGraph();



		LRefPtr<Graphics::ITexture> tex1( gd->createTexture( 64, 256, 1 ) );
		fontVLGothic->setSize( 32 );
		tex1->setFont( fontVLGothic );
		tex1->drawText( "xxxx", LRect( 0, 0, 256, 64 ) );
		//LRefPtr<Scene::Sprite3D> sp1( LN_NEW Scene::Sprite3D( sg ) );
		LRefPtr<Scene::Sprite> sp1( LN_NEW Scene::Sprite( sg ) );
		sp1->create3D(/* 256, 64 */);
		sp1->setTexture( tex1 );
		sp1->setScale( LVector3( 0.05, 0.05, 0.05 ) );
		//sp1->setCenter( LVector3( -128, -64 ) );


#endif
		

		do
		{


		} while ( fw->update() );
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}


void testInv( LMatrix* outMat, const LMatrix& mat )
{
	lnFloat coef00 = mat.m[2][2] * mat.m[3][3] - mat.m[3][2] * mat.m[2][3];
	lnFloat coef02 = mat.m[1][2] * mat.m[3][3] - mat.m[3][2] * mat.m[1][3];
	lnFloat coef03 = mat.m[1][2] * mat.m[2][3] - mat.m[2][2] * mat.m[1][3];

	lnFloat coef04 = mat.m[2][1] * mat.m[3][3] - mat.m[3][1] * mat.m[2][3];
	lnFloat coef06 = mat.m[1][1] * mat.m[3][3] - mat.m[3][1] * mat.m[1][3];
	lnFloat coef07 = mat.m[1][1] * mat.m[2][3] - mat.m[2][1] * mat.m[1][3];

	lnFloat coef08 = mat.m[2][1] * mat.m[3][2] - mat.m[3][1] * mat.m[2][2];
	lnFloat coef10 = mat.m[1][1] * mat.m[3][2] - mat.m[3][1] * mat.m[1][2];
	lnFloat coef11 = mat.m[1][1] * mat.m[2][2] - mat.m[2][1] * mat.m[1][2];

	lnFloat coef12 = mat.m[2][0] * mat.m[3][3] - mat.m[3][0] * mat.m[2][3];
	lnFloat coef14 = mat.m[1][0] * mat.m[3][3] - mat.m[3][0] * mat.m[1][3];
	lnFloat coef15 = mat.m[1][0] * mat.m[2][3] - mat.m[2][0] * mat.m[1][3];

	lnFloat coef16 = mat.m[2][0] * mat.m[3][2] - mat.m[3][0] * mat.m[2][2];
	lnFloat coef18 = mat.m[1][0] * mat.m[3][2] - mat.m[3][0] * mat.m[1][2];
	lnFloat coef19 = mat.m[1][0] * mat.m[2][2] - mat.m[2][0] * mat.m[1][2];

	lnFloat coef20 = mat.m[2][0] * mat.m[3][1] - mat.m[3][0] * mat.m[2][1];
	lnFloat coef22 = mat.m[1][0] * mat.m[3][1] - mat.m[3][0] * mat.m[1][1];
	lnFloat coef23 = mat.m[1][0] * mat.m[2][1] - mat.m[2][0] * mat.m[1][1];

	LVector4 fac0(coef00, coef00, coef02, coef03);
	LVector4 fac1(coef04, coef04, coef06, coef07);
	LVector4 fac2(coef08, coef08, coef10, coef11);
	LVector4 fac3(coef12, coef12, coef14, coef15);
	LVector4 fac4(coef16, coef16, coef18, coef19);
	LVector4 fac5(coef20, coef20, coef22, coef23);

	LVector4 vec0(mat.m[1][0], mat.m[0][0], mat.m[0][0], mat.m[0][0]);
	LVector4 vec1(mat.m[1][1], mat.m[0][1], mat.m[0][1], mat.m[0][1]);
	LVector4 vec2(mat.m[1][2], mat.m[0][2], mat.m[0][2], mat.m[0][2]);
	LVector4 vec3(mat.m[1][3], mat.m[0][3], mat.m[0][3], mat.m[0][3]);

	LVector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
	LVector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
	LVector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
	LVector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

	LVector4 signA(+1, -1, +1, -1);
	LVector4 signB(-1, +1, -1, +1);
	LMatrix inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

	LVector4 row0(inverse.m[0][0], inverse.m[1][0], inverse.m[2][0], inverse.m[3][0]);

	LVector4 dot0(mat[0] * row0);
	lnFloat dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

	if (dot1 == 0.0)
	{
		// 計算できない。D3D に合わせて、単位行列で返す
		outMat->identity();
		return;
	}

	lnFloat oneOverDeterminant = 1.0f / dot1;

	*outMat = inverse * oneOverDeterminant;
}

//-------------------------------------------------------------
void Test06_Scene()
{
#if 0 // inverse テスト
	LMatrix mat1;
	LMatrix::rotationYawPitchRoll(&mat1, 10, 20, 30);
	//mat1.m00 = 0;
	//mat1.m11 = 0;
	//mat1.m22 = 0;
	//mat1.scaling(0);

	LMatrix mat2;
	LMatrix::rotationYawPitchRoll(&mat2, 10, 20, 30);
	//mat2.m00 = 0;
	//mat2.m11 = 0;
	//mat2.m22 = 0;
	//mat2.scaling(0);
	
	LMatrix res;
	{
		System::PerfStopwatch sw;
		for (int i = 0; i < 100000; i++)
			LMatrix::inverse(&res, mat1);
	}
	res.cdump();
	
	
	{
		System::PerfStopwatch sw;
		for (int i = 0; i < 100000; i++)
		testInv(&res, mat2);
	}
	res.cdump();


	return;
#endif








	Core::ConfigData data;
	data.EnableCRTSetDebugFlags = true;
	data.BackbufferSize.set( 640, 480 );
	Core::Manager* fw = LN_NEW Core::Manager();

	//fw->initializeAudioManager( data );
	//fw->finalize();
	//fw->release();
	//return;



	try
	{
		//fw->initializeSceneManager( data );
		fw->initialize( data );

		//fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setCameraOperator(
		//	Graphics::CylinderMouseMoveCameraOperator::create());

		LRefPtr<Graphics::ITexture> tex1(Graphics::Util::createTexture( fw->getGraphicsManager(), "../LightNote.UnitTest/Data/20140309.png" ) );
			
		LRefPtr<Scene::Sprite> sprite1(LN_NEW Scene::Sprite( fw->getSceneManager()->getDefaultSceneGraph()));
		sprite1->create();
		sprite1->setTexture( tex1 );

		sprite1->release();

		fw->update();
		fw->waitRenderEnd();



		fw->update();
		fw->waitRenderEnd();



		do
		{
		} while ( fw->update() );
#if 0
#if 1
		// キャッシュテスト
		LRefPtr<Graphics::ITexture> texture2(
			fw->getGraphicsManager()->getGraphicsDevice()->createTexture( 
			"D:/Proj/TestMat/2206234.png", 0, 1, Graphics::SurfaceFormat_Unknown, lnSharingKey("D:/Proj/TestMat/2206234.png") ));
		//texture2.release();


		LRefPtr<Graphics::ITexture> texture1(
			fw->getGraphicsManager()->getGraphicsDevice()->createTexture( 
			//"D:/Proj/TestMat/2206234.png", 0, 1, LN_FMT_UNKNOWN, lnSharingKey("D:/Proj/TestMat/2206234.png")));
			"D:/GameProjects/Ota-GamePGs/Volkoff/Volkoff_vs2012/Volkoff_vs2012/Data/Graphics/Character/Char_BareHands.png" ));
#endif

#if 1
		LRefPtr<Scene::Sprite> sprite1( LN_NEW Scene::Sprite( fw->getSceneManager()->getDefaultSceneGraph() ) );
		//sprite1->create();
		sprite1->create3D( 10, 10 );
		//LRefPtr<Scene::BatchSprite3D> sprite1( LN_NEW Scene::BatchSprite3D( fw->getSceneManager()->getDefaultSceneGraph() ) );
		//sprite1->create( 10, 10);
		sprite1->setTexture( texture1 );
		//sprite1->setOpacity( 0.9);
#endif

		fw->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->setBackgroundColor(LColor(0.5, 0.5, 0.5));

		
		
		Core::Animation::FixedLengthScalar  mRoundTripValue;
		mRoundTripValue.setCapacity( 3 );
		mRoundTripValue.addKeyFrame( 0, 0.2f );
		mRoundTripValue.addKeyFrame( 60, 1.0f );
		mRoundTripValue.addKeyFrame( 120, 0.2f );
		mRoundTripValue.setEnableLoop( true );
		mRoundTripValue.setTime( 0.0 );

		LRefPtr<Scene::SpritePath> sppath1( LN_NEW Scene::SpritePath( fw->getSceneManager()->getDefaultSceneGraph() ) );
		sppath1->create( 100 );

		//LRefPtr<Scene::Sprite3D> sprite2( LN_NEW Scene::Sprite3D( fw->getSceneManager()->getDefaultSceneGraph() ) );
		//sprite2->initialize( 2, 2 );

		//LRefPtr<Scene::SceneShader> shader2( 
		//	fw->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader( "raymarching1.fx" ) );

		//sprite2->setShader( shader2 );

#if 1
		LRefPtr<Scene::Effect> effect1( LN_NEW Scene::Effect( fw->getSceneManager()->getDefaultSceneGraph() ));
		effect1->create( 
			"D:/Programing/Effekseer/Effekseer-master/Release/Sample/00_Basic/Laser01.efk" );
		effect1->play();
#endif

		//new int[3];
#if 1
		LRefPtr<Scene::Mesh> mesh1( LN_NEW Scene::Mesh( fw->getSceneManager()->getDefaultSceneGraph() ));
		mesh1->create(
			"D:/MMD/ステージ/anomarokarintou/packaged用ステージ2・泉のステージ/P・S_2_泉のステージ.x",
			0, true);
#endif
		LNAnimationLoopMode ff = LN_ANIMATIONLOOPMODE_REPEAT_LOOP;
	

		do
		{
#if 1
			mRoundTripValue.advanceTime( fw->getGameTime().getElapsedGameTime() );
            sprite1->setOpacity( mRoundTripValue.getValue() );
#endif

#if 1
			if (fw->getGameTime().getTotalFrameCount() % 30 == 0 )
			{
				effect1->play();
			}

			effect1->setPosition( LVector3( 0.1 * fw->getGameTime().getTotalFrameCount(), 0, 0 ) );
#endif
#if 1
			sppath1->drawSprite(
				LVector3( 0, 0, 0 ),
				LVector3( 0, 0, 0 ),
				texture2,
				LRectF(0, 0, 200, 200),
				LColor::WHITE );

			sppath1->drawSprite(
				LVector3( 300, 0, 0 ),
				LVector3( 0, 0, 0 ),
				texture2,
				LRectF(0, 0, 200, 200),
				LColor::WHITE );
#endif

		} while ( fw->update() );

		int a;
		//Capt( System::Environment::getWindowHandle( fw->getSystemManager()->getMainWindow() ) );
#endif
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void Test05_System()
{
	Core::ConfigData data;
	//data.EnableFullscreen = true;
	data.WindowSystemAPI = System::WindowSystemAPI_GLFW;
	Core::Manager* fw = LN_NEW Core::Manager();
	try
	{
		fw->initializeSystemManager( data );

		do
		{
		} while ( fw->update() );
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void Test04_Input()
{
	Core::ConfigData data;
	//data.WindowSystemAPI = System::WindowSystemAPI_GLFW;
	Core::Manager* fw = LN_NEW Core::Manager();
	try
	{
		fw->initializeInputManager( data );

		//fw->getInputManager()->getJoypad( 0 )->startVibration( 1000, 5000 );

		Input::GameController* input0 = fw->getInputManager()->getGameController( 0 );
		Input::GameController* input1 = fw->getInputManager()->getGameController( 1 );
		Input::Mouse* mouse = fw->getInputManager()->getMouse();
		Input::Joypad* joy = fw->getInputManager()->getJoypad(0);

		do
		{
			//printf( "." );
			if ( input0->isPress( Input::LN_BUTTON_A ) ) {
				printf( "0\n" );
			}
			if ( input1->isPress( Input::LN_BUTTON_A ) ) {
				printf( "1\n" );
			}

			if ( mouse->isPress( Input::LN_MOUSE_BUTTON_1 ) ) {
				printf( "mouse 1\n" );
			}

			if ( joy->isPress( 0 ) ) {
				printf( "joy 1\n" );
			}


			//mouse->getPoint().dump();

		} while ( fw->update() );

	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void Test03_FileIO()
{
	Core::ConfigData data;
	Core::Manager* fw = LN_NEW Core::Manager();
	fw->initializeFileManager( data );

	FileIO::InFile* file = fw->getFileManager()->createInFile( 
		"../Release/LightNote.Static.Release.lib" );
	file->load( true );
	
	printf("loading");
#if 0
	do
	{
		printf(".");
		Threading::sleep( 1 );
	} while ( fw->update() && !file->isLoadFinished() );
	printf("%d/n", file->getSize());
#endif
	file->release();

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void Test02_Graphics()
{
	
	Core::ConfigData data;
	data.BackbufferSize.set(640, 480);
	data.EnableCRTSetDebugFlags = true;
	data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
	data.WindowSystemAPI = System::WindowSystemAPI_GLFW;
	data.UpdateMode = LN_UPDATEMODE_ALWAYS;
	Core::Manager* fw = LN_NEW Core::Manager();
	try
	{
		fw->createFPSController( data );
		fw->initializeGraphicsManager( data );

		LRefPtr<Graphics::IShader> shader1( 
		////	fw->getGraphicsManager()->getGraphicsDevice()->createShader( "D:/Proj/TestMat/GodRay_old/GodRay.fx" ) );
		//	fw->getGraphicsManager()->getGraphicsDevice()->createShader( "D:/Proj/LightNote/Source/Core/Resource/Shader/SSBasic3D.lnfx" ) );
			Graphics::Util::createShader( fw->getGraphicsManager(), "D:/Proj/LightNote/Source/Core/Resource/Shader/GeometryRenderer.lnfx" ) );


		Graphics::IRenderer* r = fw->getGraphicsManager()->getGraphicsDevice()->getRenderer();
		do
		{
			fw->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->activate();

			
			

			
			r->beginScene();
			r->clear( true, true, LColor::RED );
			//
#if 0
			LRenderState state;
			state.Culling = LN_CULL_DOUBLE;

			r->setRenderState( state );
			
			fw->getGraphicsManager()->getGeometryRenderer()->begin();

			fw->getGraphicsManager()->getGeometryRenderer()->setTexture( NULL );
			fw->getGraphicsManager()->getGeometryRenderer()->setMatrix( LMatrix::IDENTITY );
			fw->getGraphicsManager()->getGeometryRenderer()->setViewProjMatrix( LMatrix::IDENTITY );

			fw->getGraphicsManager()->getGeometryRenderer()->drawRect(
				LRectF( 0, 0, 0.5, 0.5 ), LRectF( 0, 0, 1, 1 ), 0xffffffff );
			fw->getGraphicsManager()->getGeometryRenderer()->end();
#endif	
			r->endScene();
			

			fw->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->present();
			//Threading::sleep( 16 );
//			fw->getGraphicsManager()->getGraphicsDevice()->testDeviceLost();
		} while ( fw->update() );
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void Test01_Audio()
{
	FileIO::ArchiveOpenData arcData;
	arcData.Directory		= "D:/Proj/LightNote/Projects/GCC/Test";
	arcData.ArchiveName		= "Audio.lna";
	arcData.ArchiveKey		= "testpass";

	Core::ConfigData data;
	//data.ArchiveList.push_back(arcData);

	Core::Manager* fw = LN_NEW Core::Manager();
	fw->initializeAudioManager( data );
	//fw->getAudioManager()->getGameAudio()->playBGM( "D:/MMD/オーディオ/ZIGG-ZAGG.mp3" );
	//fw->getAudioManager()->getGameAudio()->playBGM( "D:/Proj/LightNotePackage/Source/Common/Media/ln23.ogg" );

	Core::Audio::Sound* sound = fw->getAudioManager()->createSound(
		"D:/Proj/LightNotePackage/Source/Common/Media/ln23.ogg", Audio::SOUNDPLAYTYPE_AUTO, false );
	sound->play();

	getchar();
	sound->release();

	
	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void TestEx01_Effekseer()
{

	Core::ConfigData data;
	data.EnableCRTSetDebugFlags = true;
	data.BackbufferSize.set( 640, 480 );
	Core::Manager* fw = LN_NEW Core::Manager();

	try
	{
		fw->initializeEffectManager( data );
		Graphics::IGraphicsDevice* gd = fw->getGraphicsManager()->getGraphicsDevice();
		//Scene::SceneGraph* sg = fw->getSceneManager()->getDefaultSceneGraph();
		Graphics::IRenderer* r = gd->getRenderer();

		LRefPtr<Effect::IEffect> effect1( fw->getEffectManager()->createEffect( 
			"D:/Programing/Effekseer/Effekseer-master/Release/Sample/00_Basic/Laser01.efk") );

		Effect::EffekseerEffectInstance* efk = dynamic_cast<Effect::EffekseerEffectInstance*>( effect1.getPtr() );
		efk->play( false );

		LMatrix view;
		LMatrix proj;
		LMatrix::viewTransformLH( 
			&view,
			LVector3( 10, 10, 10 ),
			LVector3( 0, 0, 0 ),
			LVector3( 0, 1, 0 ));
		LMatrix::perspectiveFovLH(
			&proj,
			90.0f / 180.0f * 3.14f,
			640, 480,
			1, 1000 );

		do
		{
			fw->getEffectManager()->setViewProjection( view, proj );

			fw->getEffectManager()->updateRenderContents();
			efk->advanceTime( 1.0f );
			efk->updateContext();
			efk->update();


			
			gd->getDefaultCanvas()->activate();
			r->clear( true, true, LColor(0, 0, 0), 1 );
			r->beginScene();

			//efk->getEffectCore()->getEffectEngine()->getEffekseerRenderer()->BeginRendering();
			efk->draw();
			//efk->getEffectCore()->getEffectEngine()->getEffekseerRenderer()->EndRendering();

			r->endScene();
			gd->getDefaultCanvas()->present();

			::Sleep(16);

			

		} while ( fw->update() );

#if 0
		::Effekseer::Manager* manager = fw->getEffectManager()->getEffekseerManager();
		::EffekseerRenderer::Renderer* efkRenderer = fw->getEffectManager()->getEffekseerRenderer();

		// エフェクトの読込
		Effekseer::Effect* g_effect = Effekseer::Effect::Create( 
			fw->getEffectManager()->getEffekseerManager(), 
			(const EFK_CHAR*)L"D:/Programing/Effekseer/Effekseer-master/Release/Sample/00_Basic/Laser01.efk" );

		// エフェクトの再生
		::Effekseer::Handle g_handle = fw->getEffectManager()->getEffekseerManager()->Play( g_effect, 0, 0, 0 );

		LMatrix view;
		LMatrix proj;
		LMatrix::viewTransformLH( 
			&view,
			LVector3( 10, 10, 10 ),
			LVector3( 0, 0, 0 ),
			LVector3( 0, 1, 0 ));
		LMatrix::perspectiveFovLH(
			&proj,
			90.0f / 180.0f * 3.14f,
			640, 480,
			1, 1000 );

		do
		{
			fw->getEffectManager()->setViewProjection( view, proj, true );
			gd->getDefaultCanvas()->activate();
			r->clear( true, true, LColor(0, 0, 0), 1 );
			r->beginScene();

			// エフェクトの描画開始処理を行う。
			efkRenderer->BeginRendering();

			// エフェクトの描画を行う。
			//manager->Draw();
			manager->DrawHandle( g_handle );

			// エフェクトの描画終了処理を行う。
			efkRenderer->EndRendering();

			r->endScene();
			gd->getDefaultCanvas()->present();

			::Sleep(16);

			fw->getEffectManager()->updateRenderContents();

		} while ( fw->update() );


		ES_SAFE_RELEASE( g_effect );
#endif
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}

//-------------------------------------------------------------
void TestEx02_CEGUI()
{
	Core::ConfigData data;
	data.BackbufferSize.set( 640, 480 );
	Core::Manager* fw = LN_NEW Core::Manager();


	try
	{
	//	fw->initializeGUIManager( data );
	//	fw->createFPSController( data );
	//	Graphics::IGraphicsDevice* gd = fw->getGraphicsManager()->getGraphicsDevice();
	//	Graphics::IRenderer* r = gd->getRenderer();

		fw->initialize(data);
		fw->getAudioManager()->getGameAudio()->playBGS("D:/Material/ニコニコモンズ/nc49269e.wav");

		//LRefPtr<Audio::Sound> sound1( fw->getAudioManager()->createSound( 
		//	"D:/Material/ニコニコモンズ/nc49269e.wav", Audio::SOUNDPLAYTYPE_STREAMING, false ));
		//sound1->play();
		do
		{

	//		fw->getGUIManager()->updateRenderContents();

	//		gd->getDefaultCanvas()->activate();
	//		r->clear( true, true, LColor(0, 0, 0), 1 );
	//		r->beginScene();



	//		fw->getGUIManager()->render();


	//		r->endScene();
	//		gd->getDefaultCanvas()->present();

	//		//::Sleep(16);

		} while ( fw->update() );
	}
	catch ( Base::Exception& e )
	{
		e.process();
	}

	fw->finalize();
	fw->release();
}


int main()
{
	Test10_C_API();
	//Test09_Mesh();
	//Test08_Physics();
	//Test07_Font();
	//Test06_Scene();
	//Test05_System();
	//Test04_Input();
	//Test03_FileIO();
	//Test02_Graphics();
	//Test01_Audio();
	//TestEx01_Effekseer();
	//TestEx02_CEGUI();

	return 0;
}




#endif // LNOTE_FUNCLIB_TEST

