#include "itkGlobalTractographyFilter.h"

#include <itkProgressReporter.h>

#include <mitkQBallImage.h>
#include "itkPointShell.h"

#include "GlobalTracking/BuildFibres.cpp"

#include <itkEventObject.h>

#include <QMutexLocker>
#include <itkHistogram.h>
#include <itkListSampleToHistogramGenerator.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/vnl_vector_fixed.h>
#include <itkzlib/zlib.h>

#include "GlobalTracking/reparametrize_arclen2.cpp"
#include <fstream>

struct LessDereference {
  template <class T>
      bool operator()(const T * lhs, const T * rhs) const {
    return *lhs < *rhs;
  }
};

namespace itk{

  template< class TInputOdfImage, class TInputROIImage >
  GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::GlobalTractographyFilter():
      m_TempStart(0.1),
      m_TempEnd(0.001),
      m_NumIt(500000),
      m_ParticleWeight(0),
      m_ParticleWidth(0),
      m_ParticleLength(0),
      m_ChempotConnection(10),
      m_ChempotParticle(0),
      m_InexBalance(0),
      m_Chempot2(0.2),
      m_FiberLength(10),
      m_AbortTracking(false),
      m_NumConnections(0),
      m_NumParticles(0),
      m_NumAcceptedFibers(0),
      m_CurrentStep(0),
      m_SubtractMean(true),
      m_BuildFibers(false),
      m_Sampler(NULL),
      m_Steps(10),
      m_Memory(0),
      m_ProposalAcceptance(0)
  {
    //this->m_MeasurementFrame.set_identity();
    this->SetNumberOfRequiredInputs(2); //Filter needs a DWI image + a Mask Image
  }

  template< class TInputOdfImage, class TInputROIImage >
  GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::~GlobalTractographyFilter(){
    delete BESSEL_APPROXCOEFF;
    if (m_Sampler!=NULL)
      delete m_Sampler;
  }

  template< class TInputOdfImage, class TInputROIImage >
  void
      GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::ComputeFiberCorrelation(){

//    float bD = 15;

//    vnl_matrix_fixed<double, 3, QBALL_ODFSIZE> bDir =
//        *itk::PointShell<QBALL_ODFSIZE, vnl_matrix_fixed<double, 3, QBALL_ODFSIZE> >::DistributePointShell();

//    const int N = QBALL_ODFSIZE;

//    vnl_matrix_fixed<double, QBALL_ODFSIZE, 3> temp = bDir.transpose();
//    vnl_matrix_fixed<double, N, N> C = temp*bDir;
//    vnl_matrix_fixed<double, N, N> Q = C;
//    vnl_vector_fixed<double, N> mean;
//    for(int i=0; i<N; i++)
//    {
//      double tempMean = 0;
//      for(int j=0; j<N; j++)
//      {
//        C(i,j) = abs(C(i,j));
//        Q(i,j) = exp(-bD * C(i,j) * C(i,j));
//        tempMean += Q(i,j);
//      }
//      mean[i] = tempMean/N;
//    }

//    vnl_matrix_fixed<double, N, N> repMean;
//    for (int i=0; i<N; i++)
//      repMean.set_row(i, mean);
//    Q -= repMean;

//    vnl_matrix_fixed<double, N, N> P = Q*Q;

//    std::vector<const double *> pointer;
//    pointer.reserve(N*N);
//    double * start = C.data_block();
//    double * end =  start + N*N;
//    for (double * iter = start; iter != end; ++iter)
//    {
//      pointer.push_back(iter);
//    }
//    std::sort(pointer.begin(), pointer.end(), LessDereference());

//    vnl_vector_fixed<double,N*N> alpha;
//    vnl_vector_fixed<double,N*N> beta;
//    for (int i=0; i<N*N; i++) {
//      alpha(i) = *pointer[i];
//      beta(i)  = *(P.data_block()+(pointer[i]-start));
//    }

//    double nfac = sqrt(beta(N*N-1));
//    beta = beta / (nfac*nfac);
//    Q = Q / nfac;

//    double sum = 0;
//    for(int i=0; i<N; i++)
//    {
//      sum += Q(0,i);
//    }
//    // if left to default 0
//    // then mean is not substracted in order to correct odf integral
//    // this->m_Meanval_sq = (sum*sum)/N;

//    vnl_vector_fixed<double,N*N> alpha_0;
//    vnl_vector_fixed<double,N*N> alpha_2;
//    vnl_vector_fixed<double,N*N> alpha_4;
//    vnl_vector_fixed<double,N*N> alpha_6;
//    for(int i=0; i<N*N; i++)
//    {
//      alpha_0(i) = 1;
//      alpha_2(i) = alpha(i)*alpha(i);
//      alpha_4(i) = alpha_2(i)*alpha_2(i);
//      alpha_6(i) = alpha_4(i)*alpha_2(i);
//    }

//    vnl_matrix_fixed<double, N*N, 4> T;
//    T.set_column(0,alpha_0);
//    T.set_column(1,alpha_2);
//    T.set_column(2,alpha_4);
//    T.set_column(3,alpha_6);

//    vnl_vector_fixed<double,4> coeff = vnl_matrix_inverse<double>(T).pinverse()*beta;

//    MITK_INFO << "Bessel oefficients: " << coeff;

    BESSEL_APPROXCOEFF = new float[4];

//    BESSEL_APPROXCOEFF[0] = coeff(0);
//    BESSEL_APPROXCOEFF[1] = coeff(1);
//    BESSEL_APPROXCOEFF[2] = coeff(2);
//    BESSEL_APPROXCOEFF[3] = coeff(3);
    BESSEL_APPROXCOEFF[0] = -0.1714;
    BESSEL_APPROXCOEFF[1] = 0.5332;
    BESSEL_APPROXCOEFF[2] = -1.4889;
    BESSEL_APPROXCOEFF[3] = 2.0389;
  }

  // build fibers from tracking result
  template< class TInputOdfImage, class TInputROIImage >
  void
      GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::BuildFibers(float* points, int numPoints)
  {
    MITK_INFO << "Building fibers ...";

    typename InputQBallImageType::Pointer odfImage
        = dynamic_cast<InputQBallImageType*>(this->GetInput(0));
    double spacing[3];
    spacing[0] = odfImage->GetSpacing().GetElement(0);
    spacing[1] = odfImage->GetSpacing().GetElement(1);
    spacing[2] = odfImage->GetSpacing().GetElement(2);

    // initialize array of particles
    CCAnalysis ccana(points, numPoints, spacing);

    // label the particles according to fiber affiliation and return number of fibers
    int numFibers = ccana.iterate(m_FiberLength);

    if (numFibers<=0){
      MITK_INFO << "0 fibers accepted";
      return;
    }

    // fill output datastructure
    m_FiberBundle.clear();
    for (int i = 0; i < numFibers; i++)
    {
      vector< Particle* >* particleContainer = ccana.m_FiberContainer->at(i);

      // resample fibers
      std::vector< Particle* >* pCon = ResampleFibers(particleContainer, 0.9*spacing[0]);

      FiberTractType tract;
      for (int j=0; j<pCon->size(); j++)
      {
        Particle* particle = pCon->at(j);
        pVector p = particle->R;

        itk::Point<float, 3> point;
        point[0] = p[0]-0.5;
        point[1] = p[1]-0.5;
        point[2] = p[2]-0.5;
        tract.push_back(point);
        delete(particle);
      }
      m_FiberBundle.push_back(tract);
      delete(pCon);
    }
    m_NumAcceptedFibers = numFibers;
    MITK_INFO << "itkGlobalTractographyFilter: "  << numFibers << " fibers accepted";
  }

  // fill output fiber bundle datastructure
  template< class TInputOdfImage, class TInputROIImage >
  typename GlobalTractographyFilter< TInputOdfImage, TInputROIImage >::FiberBundleType*
      GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::GetFiberBundle()
  {
    if (!m_AbortTracking)
    {
      m_BuildFibers = true;
      while (m_BuildFibers){}
    }

    return &m_FiberBundle;
  }

  // get memory allocated for particle grid
  template< class TInputOdfImage, class TInputROIImage >
  float
      GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::GetMemoryUsage()
  {
    if (m_Sampler!=NULL)
      return m_Sampler->m_ParticleGrid.GetMemoryUsage();
    return 0;
  }

  // perform global tracking
  template< class TInputOdfImage, class TInputROIImage >
  void
      GlobalTractographyFilter< TInputOdfImage, TInputROIImage >
      ::GenerateData(){

    // input qball image
    m_ItkQBallImage = dynamic_cast<InputQBallImageType*>(this->GetInput(0));

    // approximationscoeffizienten der
    // teilchenkorrelationen im orientierungsraum
    // 4er vektor
    ComputeFiberCorrelation();

    // image sizes and spacing
    int qBallImageSize[4] = {QBALL_ODFSIZE,
                   m_ItkQBallImage->GetLargestPossibleRegion().GetSize().GetElement(0),
                   m_ItkQBallImage->GetLargestPossibleRegion().GetSize().GetElement(1),
                   m_ItkQBallImage->GetLargestPossibleRegion().GetSize().GetElement(2)};
    double qBallImageSpacing[3] = {m_ItkQBallImage->GetSpacing().GetElement(0),m_ItkQBallImage->GetSpacing().GetElement(1),m_ItkQBallImage->GetSpacing().GetElement(2)};

    // make sure image has enough slices
    if (qBallImageSize[1]<3 || qBallImageSize[2]<3 || qBallImageSize[3]<3)
    {
      MITK_INFO << "image size < 3 not supported";
      return;
    }

    // calculate rotation matrix
    vnl_matrix_fixed<double, 3, 3>  directionMatrix = m_ItkQBallImage->GetDirection().GetVnlMatrix();
    vnl_vector_fixed<double, 3> d0 = directionMatrix.get_column(0); d0.normalize();
    vnl_vector_fixed<double, 3> d1 = directionMatrix.get_column(1); d1.normalize();
    vnl_vector_fixed<double, 3> d2 = directionMatrix.get_column(2); d2.normalize();
    directionMatrix.set_column(0, d0);
    directionMatrix.set_column(1, d1);
    directionMatrix.set_column(2, d2);
    vnl_matrix_fixed<double, 3, 3> I = directionMatrix*directionMatrix.transpose();
    if(!I.is_identity(mitk::eps)){
      MITK_INFO << "Image direction is not a rotation matrix. Tracking not possible!";
      return;
    }

    // generate local working copy of image buffer
    int bufferSize = qBallImageSize[0]*qBallImageSize[1]*qBallImageSize[2]*qBallImageSize[3];
    float* qBallImageBuffer = (float*) m_ItkQBallImage->GetBufferPointer();
    float* workingQballImage = new float[bufferSize];
    for (int i=0; i<bufferSize; i++)
      workingQballImage[i] = qBallImageBuffer[i];

    // perform mean subtraction on odfs
    if (m_SubtractMean)
    {
      float sum = 0;
      for (int i=0; i<bufferSize; i++)
      {
        if (qBallImageSize[0]>0 && i%qBallImageSize[0] == 0 && i>0)
        {
          sum /= qBallImageSize[0];
          for (int j=i-qBallImageSize[0]; j<i; j++){
            workingQballImage[j] -= sum;
          }
          sum = 0;
        }
        sum += workingQballImage[i];
      }
    }

    // mask image
    int maskImageSize[3];
    float *mask;
    if(m_MaskImage.IsNotNull())
    {
      mask = (float*) m_MaskImage->GetBufferPointer();
      maskImageSize[0] = m_MaskImage->GetLargestPossibleRegion().GetSize().GetElement(0);
      maskImageSize[1] = m_MaskImage->GetLargestPossibleRegion().GetSize().GetElement(1);
      maskImageSize[2] = m_MaskImage->GetLargestPossibleRegion().GetSize().GetElement(2);
    }
    else
    {
      mask = 0;
      maskImageSize[0] = qBallImageSize[1];
      maskImageSize[1] = qBallImageSize[2];
      maskImageSize[2] = qBallImageSize[3];
    }
    int mask_oversamp_mult = maskImageSize[0]/qBallImageSize[1];

    // load lookuptable
    ifstream BaryCoords;
    BaryCoords.open("FiberTrackingLUTBaryCoords.bin", ios::in | ios::binary);
    float* coords;
    if (BaryCoords.is_open())
    {
      float tmp;
      coords = new float [1630818];
      BaryCoords.seekg (0, ios::beg);
      for (int i=0; i<1630818; i++)
      {
        BaryCoords.read((char *)&tmp, sizeof(tmp));
        coords[i] = tmp;
      }
      BaryCoords.close();
    }
    else
    {
      MITK_INFO << "Unable to open barycoords file";
      return;
    }

    ifstream Indices;
    Indices.open("FiberTrackingLUTIndices.bin", ios::in | ios::binary);
    int* ind;
    if (Indices.is_open())
    {
      int tmp;
      ind = new int [1630818];
      Indices.seekg (0, ios::beg);
      for (int i=0; i<1630818; i++)
      {
        Indices.read((char *)&tmp, 4);
        ind[i] = tmp;
      }
      Indices.close();
    }
    else
    {
      MITK_INFO << "Unable to open indices file";
      return;
    }

    // initialize sphere interpolator with lookuptables
    SphereInterpolator *sinterp = new SphereInterpolator(coords, ind, QBALL_ODFSIZE, 301, 0.5);

    // get paramters
    float minSpacing;
    if(qBallImageSpacing[0]<qBallImageSpacing[1] && qBallImageSpacing[0]<qBallImageSpacing[2])
      minSpacing = qBallImageSpacing[0];
    else if (qBallImageSpacing[1] < qBallImageSpacing[2])
      minSpacing = qBallImageSpacing[1];
    else
      minSpacing = qBallImageSpacing[2];

    if(m_ParticleLength == 0)
      m_ParticleLength = 1.5*minSpacing;
    if(m_ParticleWidth == 0)
      m_ParticleWidth = 0.5*minSpacing;
    if(m_ParticleWeight == 0)
      m_ParticleWeight = 0.01;
    m_CurrentStep = 0;
    m_Memory = 0;

    float cellsize = 2*m_ParticleLength;
    float curvatureHardThreshold = 0.7;
    float alpha = log(m_TempEnd/m_TempStart);
    m_Steps = m_NumIt/10000;
    if (m_Steps<10)
      m_Steps = 10;
    if (m_Steps>m_NumIt)
    {
      MITK_INFO << "not enough iterations!";
      return;
    }
    unsigned long singleIts = (unsigned long)((1.0*m_NumIt) / (1.0*m_Steps));

    // setup metropolis hastings sampler
    MITK_INFO << "itkGlobalTractographyFilter: setting up MH-sampler";
    if (m_Sampler!=NULL)
      delete m_Sampler;
    m_Sampler = new RJMCMC(NULL, 0, workingQballImage, qBallImageSize, qBallImageSpacing, cellsize);

    // setup energy computer
    MITK_INFO << "itkGlobalTractographyFilter: setting up Energy-computer";
    EnergyComputer encomp(workingQballImage,qBallImageSize,qBallImageSpacing,sinterp,&(m_Sampler->m_ParticleGrid),mask,mask_oversamp_mult, directionMatrix);
    encomp.setParameters(m_ParticleWeight,m_ParticleWidth,m_ChempotConnection*m_ParticleLength*m_ParticleLength,m_ParticleLength,curvatureHardThreshold,m_InexBalance,m_Chempot2);
    m_Sampler->SetEnergyComputer(&encomp);
    m_Sampler->SetParameters(m_TempStart,singleIts,m_ParticleLength,curvatureHardThreshold,m_ChempotParticle);

    // main loop
    for( int step = 0; step < m_Steps; step++ )
    {
      if (m_AbortTracking)
        break;

      m_CurrentStep = step+1;
      float temperature = m_TempStart * exp(alpha*(((1.0)*step)/((1.0)*m_Steps)));
      MITK_INFO << "iterating step " << m_CurrentStep;

      m_Sampler->SetTemperature(temperature);
      m_Sampler->Iterate(&m_ProposalAcceptance, &m_NumConnections, &m_NumParticles, &m_AbortTracking);

      MITK_INFO << "proposal acceptance: " << 100*m_ProposalAcceptance << "%";
      MITK_INFO << "particles: " << m_NumParticles;
      MITK_INFO << "connections: " << m_NumConnections;
      MITK_INFO << "progress: " << 100*(float)step/m_Steps << "%";

      if (m_BuildFibers)
      {
        int numPoints = m_Sampler->m_ParticleGrid.pcnt;
        float* points = new float[numPoints*m_Sampler->m_NumAttributes];
        m_Sampler->WriteOutParticles(points);
        BuildFibers(points, numPoints);
        delete points;
        m_BuildFibers = false;
      }
    }

    int numPoints = m_Sampler->m_ParticleGrid.pcnt;
    float* points = new float[numPoints*m_Sampler->m_NumAttributes];
    m_Sampler->WriteOutParticles(points);
    BuildFibers(points, numPoints);
    delete points;

    delete sinterp;
    delete coords;
    delete ind;
    delete workingQballImage;
    m_AbortTracking = true;
    m_BuildFibers = false;

    MITK_INFO << "done generate data";
  }
}




