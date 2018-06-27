pipeline {
  agent {
    docker {
      label 'Bitprim_Slave'
      image 'ubuntu:18.10'
      args '''-u root -v /var/run/docker.sock:/var/run/docker.sock
-v jenkins_build:/var/jenkins_home'''
    }

  }
  stages {
    stage('Requirements') {
      steps {
        sh '''#Install Requirements on Ubuntu18.10 docker image
apt-get update
apt-get install build-essential -y
apt-get install gcc
apt-get install cmake -y
apt-get install python -y
apt-get install python-pip -y
pip install conan --upgrade
conan remote add bitprim https://api.bintray.com/conan/bitprim/bitprim'''
      }
    }
    stage('Build') {
      steps {
        sh '''env.BRANCH_NAME
printenv

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
            sh 'build/bin/bitprim-cli seed_to_wallet fffb587496cc54912bbcef874fa9a61a true false'
          }
        }
        stage('Run Create Txn') {
          steps {
            sh 'build/bin/bitprim-cli create_txn -i "980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393:1" -o "mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:199999000"'
          }
        }
      }
    }
    stage('Finish') {
      steps {
        timeout(time: 5, unit: 'MINUTES') {
          input 'Do you want to print a message?'
        }

        echo 'Le dieron al OK'
      }
    }
  }
}