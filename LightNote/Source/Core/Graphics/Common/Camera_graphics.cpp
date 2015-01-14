//==============================================================================
// Camera 
//==============================================================================

#include "stdafx.h"
#include "Camera.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ Camera
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Camera::Camera()
        : mCameraType		( CameraType_PositionAndTarget )
		, mCameraOperator   ( NULL )
        , mProjection2D     ( false )
        , mModified         ( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Camera::~Camera()
    {
        LN_SAFE_RELEASE( mCameraOperator );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::initialize( bool projection_2d )
    {
        mProjection2D = projection_2d;
        if ( mProjection2D )
        {
			mCameraType = CameraType_PositionAndAngle;
            mPosition.Set( 0.0f, 0.0f, 0.0f );
            mNearClip = -1000.0f;
            mFarClip = 1000.0f;
        }
        else
        {
			mCameraType = CameraType_PositionAndTarget;
            mPosition.Set( 0.0f, 0.0f, -10.0f );
            mNearClip = 1.0f;
            mFarClip = 1000.0f;
        }

        mLookAt.Set( 0, 0, 0 );
		mUpDirection.Set( 0, 1, 0 );
        mDirection = mLookAt - mPosition;
        mDirection.Normalize();

        mFovY = LMath::PI * 0.125f;

		mModified = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::setPosition( const LVector3& pos )
	{
		mPosition = pos;
        mDirection = mLookAt - mPosition;
        mDirection.Normalize();
		mModified = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::setLookAt( const LVector3& at )
	{
		mLookAt = at;
        mDirection = mLookAt - mPosition;
        mDirection.Normalize();
		mModified = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::setUpDirection( const LVector3& up )
	{
		mUpDirection = up;
		mModified = true;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::setFovY( lnFloat fov_y )
    {
        mFovY = fov_y;
        mModified = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::setNearClip( lnFloat near_clip )
    {
        mNearClip = near_clip;
        mModified = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::setFarClip( lnFloat far_clip )
    {
        mFarClip = far_clip;
        mModified = true;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::setCameraOperator( CameraOperator* camera_operator )
    {
        mCameraOperator = camera_operator;
        if ( mCameraOperator )
            mCameraOperator->mTargetCamera = this;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Camera::convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos, const LVector2& view_size ) const
    {
		_updateMatrix();

		*out = LVector3::TransformCoord(world_pos, mViewProjMatrix);
        
        out->Y *= -1.0f;  // y は下が + なので上下反転

        out->X = ( out->X + 1.0f ) * 0.5f;    // 0.0～2.0  にしたあと、0.0～1.0 にする
        out->Y = ( out->Y + 1.0f ) * 0.5f;    // (左上を原点にしたあと、)

        out->X *= view_size.X;
        out->Y *= view_size.Y;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void Camera::_updateMatrix() const
	{
        if ( mModified )
		{
            // 2D カメラ用
            if ( mProjection2D )
            {
				// 種別に応じて View 行列を作る
				switch ( mCameraType )
				{
					default:
					case CameraType_PositionAndTarget:
					case CameraType_PositionAndAngle:
					{
						mViewMatrix = LMatrix::RotationYawPitchRoll(mAngle.Y, mAngle.X, mAngle.Z);
						mViewMatrix.Translation(-mPosition);
						break;
					}
					case CameraType_TransformMatrix:
					{
						mViewMatrix = mMatrix;
						break;
					}
				}

                LMatrixUtils::perspective2DLH( 
                    &mProjectionMatrix,
                    mViewSize.X, mViewSize.Y, mNearClip, mFarClip );

                mViewProjMatrix = mViewMatrix * mProjectionMatrix;

				mViewProjMatrixInv = LMatrix::Inverse(mViewMatrix);

                mViewFrustum.create2DProjection( mViewSize.X, mViewSize.Y, mNearClip, mFarClip );
                
            }
            // 3D カメラ用
            else
            {
				// 種別に応じて View 行列を作る
				switch ( mCameraType )
				{
					default:
					case CameraType_PositionAndTarget:
					{
						mViewMatrix = LMatrix::LookAtLH(mPosition, mLookAt, mUpDirection);
						break;
					}
					case CameraType_PositionAndAngle:
					{
						LMatrix rotMat = LMatrix::RotationYawPitchRoll(mAngle.Y, mAngle.X, mAngle.Z);
						
						// (0,0,1) を正面、(0,1,0) を上とし、View 行列を作る
						LVector3 front = LVector3::TransformCoord(LVector3::UnitZ, rotMat);
						LVector3 up = LVector3::TransformCoord(LVector3::UnitY, rotMat);

						mViewMatrix = LMatrix::LookAtLH(mPosition, mPosition + front, up);
						break;
					}
					case CameraType_TransformMatrix:
					{
						LVector3 front = LVector3::TransformCoord(LVector3::UnitZ, mMatrix);
						LVector3 up = LVector3::TransformCoord(LVector3::UnitY, mMatrix);
						mViewMatrix = LMatrix::LookAtLH(mPosition, front, up);
						break;
					}
				}

                //LMatrix::viewTransformLH( &mViewMatrix, mPosition, mLookAt, mUpDirection );
				mProjectionMatrix = LMatrix::PerspectiveFovLH(mFovY, mViewSize.X / mViewSize.Y, mNearClip, mFarClip);
               
                mViewProjMatrix = mViewMatrix * mProjectionMatrix;

				mViewProjMatrixInv = LMatrix::Inverse(mViewMatrix);

                //LMath::MatrixOrthoLH( &mOrthoMatrix, mViewSize.x, mViewSize.y, 1000.0f, 0.0f );

                mViewFrustum.create( mFovY, mViewSize.X / mViewSize.Y, mNearClip, mFarClip );

            }

            mModified = false;
        }
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool Camera::onEvent( const System::EventArgs& e )
    {
        if ( mCameraOperator && mCameraOperator->onEvent( e ) ) return true;
        return false;
    }

//==============================================================================
// ■ CylinderMouseMoveCameraOperator
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    CylinderMouseMoveCameraOperator::CylinderMouseMoveCameraOperator()
    {
        mMouseData.RDrag = false;
        mMouseData.MDrag = false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    bool CylinderMouseMoveCameraOperator::onEvent( const System::EventArgs& e )
    {
        const System::MouseEventArgs& ev_mouse = (const System::MouseEventArgs&)(e);

        switch ( e.Type )
        {
            /////////////////////////////////////////////// マウスボタンが押された
            case LNEV_MOUSE_DOWN:
		    {
                // 右ボタン
                if ( ev_mouse.Button == LN_MOUSE_RIGHT )
                {
                    mMouseData.RDrag = true;

                    // マウス位置記憶
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
                    return true;
                    
                }
                // 中ボタン
                if ( ev_mouse.Button == LN_MOUSE_MIDDLE )
                {
                    mMouseData.MDrag = true;
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
                    return true;
                }

			    break;
		    }

            /////////////////////////////////////////////// マウスボタンが離された
            case LNEV_MOUSE_UP:
		    {
                // 右ボタン
                if ( ev_mouse.Button == LN_MOUSE_RIGHT )
                {
                    mMouseData.RDrag = false;
                    return true;
                }
                // 中ボタン
                if ( ev_mouse.Button == LN_MOUSE_MIDDLE )
                {
                    mMouseData.MDrag = false;
                    return true;
                }
			    break;
		    }

		    /////////////////////////////////////////////// マウス移動
            case LNEV_MOUSE_MOVE:
		    {
                // 右ボタンドラッグ中の場合
	            if ( mMouseData.RDrag )
	            {
                    const LVector2& view_size = getTargetCamera()->getViewSize();

		            _doMouseMoveR(
			            static_cast< lnFloat >( ev_mouse.X - mMouseData.PrevPos.x ),
			            static_cast< lnFloat >( ev_mouse.Y - mMouseData.PrevPos.y ),
                        view_size.X, view_size.Y );
		            mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
		            return true;
	            }
				// 中ボタンドラッグ中の場合
	            if ( mMouseData.MDrag )
	            {
		            _doMouseMoveM(
			            static_cast< lnFloat >( ev_mouse.X - mMouseData.PrevPos.x ),
			            static_cast< lnFloat >( ev_mouse.Y - mMouseData.PrevPos.y ) );
                    mMouseData.PrevPos.x = ev_mouse.X;
                    mMouseData.PrevPos.y = ev_mouse.Y;
		            return true;
	            }

			    break;
		    }
			
            /////////////////////////////////////////////// マウスホイールが操作された
		    case LNEV_MOUSE_WHEEL:
		    {
                _doMouseWheelEvent( ev_mouse.Delta );
			    return true;
		    }
        }

        return false;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CylinderMouseMoveCameraOperator::_doMouseMoveR( lnFloat dx, lnFloat dy, lnFloat width, lnFloat height )
	{
		LVector3 view;
		LVector3 vup = getTargetCamera()->getUpDirection();
        LVector3 pos = getTargetCamera()->getPosition();
        LVector3 look_at = getTargetCamera()->getLookAt();
		vup.Normalize();

		// 注視点中心回転
		if ( 1 )
		{
			view = pos - look_at;
		}
		// 視点中心回転
		else
		{
			view = look_at - pos;
		}

		LMatrix m;
		lnFloat d;

		if ( dx != 0 )
		{
			d = LMath::PI * dx / width;
			// Y 軸周りの回転を逆にする場合 (右手系)
			if ( 0 ) // CAMTYPE_REV_ROT_Y
			{
				d = -d;
			}
			
			// vup を軸にする場合
			//if ( m_type & CAMTYPE_AROUND_UP )
			//	D3DXMatrixRotationAxis( &m, &vup, D3DXToRadian(d) );
			//else
			//	D3DXMatrixRotationY( &m, D3DXToRadian(d) );

			m.RotationY( d );
			view.TransformCoord( m );
		}
		if ( dy != 0 )
		{
			// 球タイプ
			if ( 1 )
			{
				LVector3 vaxis = LVector3::Cross(vup, view);
				vaxis.Normalize();
				d = -( LMath::PI * dy / height );
                
                m = LMatrix::RotationAxis(vaxis, d );
				view.TransformCoord( m );


				//D3DXVec3Cross( &vaxis, &vup, &view );
				//D3DXVec3Normalize( &vaxis, &vaxis );
				//d = (float)180.0 * (float)dy / (float)prc->bottom;
				//D3DXMatrixRotationAxis( &m, &vaxis, D3DXToRadian(d) );
				//D3DXVec3TransformNormal( &view, &view, &m );
			}
			else
			{
				//if( m_type & CAMTYPE_AROUND_UP )
				//	view += ( vup * (float)dy );
				//else
				//	view.y += (float)dy;
			}
		}

		// 注視点中心回転
		if ( 1 )
		{
            LVector3 old = pos;

			pos = look_at + view;

            // 上または下にドラッグし続けた場合、中天を通り過ぎたところで
            // XZ上の反対側の象限にカメラが移動して画面がちらちらするのを防ぐ処理
            if ( ( ( old.X < 0 && pos.X > 0 ) || ( old.X > 0 && pos.X < 0 ) ) &&
                 ( ( old.Z < 0 && pos.Z > 0 ) || ( old.Z > 0 && pos.Z < 0 ) ) )
            {
                pos = old;
            }

		}
		// 視点中心回転
		else
		{
			pos += view;
		}

        getTargetCamera()->setPosition( pos );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CylinderMouseMoveCameraOperator::_doMouseMoveM( lnFloat dx, lnFloat dy )
    {
        LVector3 pos = getTargetCamera()->getPosition();
        LVector3 look_at = getTargetCamera()->getLookAt();

        lnFloat s = 0.00175f * ( pos - look_at ).GetLength();

		LVector3 view;
		LMatrix mat = LMatrix::Inverse(getTargetCamera()->getViewMatrix());
		mat.M[3][0] = mat.M[3][1] = mat.M[3][2] = 0.0f;
		view.X = -dx * s;
		view.Y = dy * s;
		view.Z = 0.f;
		view.TransformCoord( mat );

        getTargetCamera()->setPosition( pos + view );
        getTargetCamera()->setLookAt( look_at + view );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void CylinderMouseMoveCameraOperator::_doMouseWheelEvent( int pos )
    {
        LVector3 view = getTargetCamera()->getPosition() - getTargetCamera()->getLookAt();
		if ( pos >= 0 )
		{
			view *= 0.9f;
		}
		else
		{
			view *= 1.1f;
		}
		getTargetCamera()->setPosition( getTargetCamera()->getLookAt() + view );
    }

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================