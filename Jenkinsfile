node('Bitprim_Slave') {
pipeline {
  agent {
      docker {
        image 'ubuntu:18.10'
        args '-u 0:0'
      }
    }
  
  stages {
    stage('Init') {
      steps {
        sh '''echo PATH = ${PATH}
echo $HOSTNAME
whoami
echo whoami

apt-get update
apt-get install build-essential -y
apt-get install gcc
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
}
