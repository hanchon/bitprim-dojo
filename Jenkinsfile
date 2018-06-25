pipeline {
  agent {
    docker {
      image 'gcc:5'
    }

  }
  stages {
    stage('Init') {
      steps {
        sh '''echo PATH = ${PATH}

apt-get update
apt-get install build-essential -y
apt-get install cmake -y
apt-get install python -y
apt-get install python-pip -y
pip install conan'''
      }
    }
    stage('Build') {
      steps {
        sh '''ls
mkdir build
cd build
conan install ..
cmake ..
cmake --build ./ --target bitprim-dojo -- -j 2
cmake --build ./ --target bitprim-cli -- -j 2'''
      }
    }
  }
}