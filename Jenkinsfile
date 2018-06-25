pipeline {
  agent {
    docker {
      image 'ubuntu:18.10'
      args '-u root'
    }

  }
  stages {
    stage('Init') {
      steps {
        sh '''echo PATH = ${PATH}
echo $HOSTNAME
whoami

apt-get update
apt-get install build-essential -y
apt-get install gcc
apt-get install cmake -y
apt-get install python -y
apt-get install python-pip -y
pip install conan
conan remote add bitprim https://api.bintray.com/conan/bitprim/bitprim'''
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