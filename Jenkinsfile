pipeline {
  agent {
    docker {
      image 'ubuntu:18.10'
    }

  }
  stages {
    stage('Init') {
      steps {
        sh '''echo PATH = ${PATH}

sudo apt-get update
sudo apt-get install build-essential -y
sudo apt-get install gcc
sudo apt-get install cmake -y
sudo apt-get install python -y
sudo apt-get install python-pip -y
sudo pip install conan'''
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