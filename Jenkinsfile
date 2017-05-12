pipeline {
	agent {
		label 'x86_64'
	}
	post {
	    failure {
	    	updateGitlabCommitStatus name: 'build', state: 'failed'
	    }
	    success {
	    	updateGitlabCommitStatus name: 'build', state: 'success'
	    }
	}
	options {
	    gitLabConnection('AVS GitLab')
	    buildDiscarder(logRotator(numToKeepStr:'10'))
	    skipDefaultCheckout()
	}
	triggers {
	    gitlab(triggerOnPush: true,
	    	   triggerOnMergeRequest: true,
	    	   triggerOpenMergeRequestOnPush: 'both',
	    	   branchFilterType: 'All')
	    pollSCM('H/10 * * * *')
	}

	stages {
		stage('checkout') {
			steps {
				dir('outpost-core') {
		        	// https://hbryavsci1l.hb.dlr.de:8929/avionics-software-open/outpost-core.git
		        	checkout scm
		        }
		        dir('scons-build-tools') {
		            git credentialsId: 'd895b75a-06cc-4446-a936-afe31d36d02b',
		            	url: 'https://hbryavsci1l.hb.dlr.de:8929/avionics-software-open/scons-build-tools.git'
		        }
			}
	    }
		stage("build") {
			steps {
				dir('outpost-core') {
					sh 'make test'
				}
			}
		}
		stage("test") {
			steps {
				dir('outpost-core') {
					step([$class: 'XUnitPublisher',
	                    testTimeMargin: '3000',
	                    thresholdMode: 1,
	                    thresholds: [
	                        [
	                            $class: 'FailedThreshold',
	                            failureNewThreshold: '',
	                            failureThreshold: '0',
	                            unstableNewThreshold: '',
	                            unstableThreshold: ''
	                        ],
	                        [
	                            $class: 'SkippedThreshold',
	                            failureNewThreshold: '',
	                            failureThreshold: '',
	                            unstableNewThreshold: '',
	                            unstableThreshold: '0']
	                        ],
	                        tools: [[
	                            $class: 'GoogleTestType',
	                            deleteOutputFiles: true,
	                            failIfNotNew: true,
	                            pattern: 'build/test/*.xml',
	                            skipNoTestFiles: false,
	                            stopProcessingIfError: true]]])
				}
			}
		}
	}
}
