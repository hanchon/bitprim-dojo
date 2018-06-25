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
'''
      }
    }
    stage('Build') {
      steps {
        sh 'ls'
      }
    }
  }
}