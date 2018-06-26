pipeline {
  agent {
    node {
      label 'Bitprim_Slave'
    }

  }
  stages {
    stage('Init') {
      steps {
        sh '''echo PATH = ${PATH}
echo $HOSTNAME
whoami

#docker run -t -d -u root ubuntu:18.10 cat
#echo PATH = ${PATH}
#echo $HOSTNAME
#whoami

apt-get update
apt-get install build-essential -y
apt-get install gcc
apt-get install cmake -y
apt-get install python -y
apt-get install python-pip -y
pip install conan --upgrade
#conan remote add bitprim https://api.bintray.com/conan/bitprim/bitprim'''
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
        archiveArtifacts 'build/bin/*'
      }
    }
    stage('Run Create Addr') {
      parallel {
        stage('Run Create Addr') {
          steps {
            sh '''ls
build/bin/bitprim-cli seed_to_wallet fffb587496cc54912bbcef874fa9a61a true false'''
          }
        }
        stage('Run Create Txn') {
          steps {
            sh '''ls
build/bin/bitprim-cli create_txn -i "980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393:1" -o "mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:199999000"'''
          }
        }
      }
    }
  }
}