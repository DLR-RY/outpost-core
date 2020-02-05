#!/usr/bin/env groovy

updateGitlabCommitStatus name: 'build', state: 'pending'

pipeline {
    agent {
        label 'x86_64'
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
               pendingBuildName: "Jenkins",
               branchFilterType: 'All')
        pollSCM('H/60 * * * *')
    }
    stages {
        stage('checkout') {
            steps {
                updateGitlabCommitStatus name: 'build', state: 'running'

                dir('outpost-core') {
                    // https://hbryavsci1l.hb.dlr.de:8929/avionics-software-open/outpost-core.git
                    checkout scm
                }
                dir('scons-build-tools') {
                    git credentialsId: 'jenkins-ssh-gitlab',
                        url: 'ssh://git@hbryavsci1l.hb.dlr.de:10022/avionics-software-open/scons-build-tools.git'
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
        stage("cppcheck") {
            steps {
                parallel (
                    "cppcheck modules": {
                        dir('outpost-core') {
                            sh 'make cppcheck'
                        }
                    },
                    "cppcheck tests": {
                        dir('outpost-core') {
                            sh 'make cppcheck-tests'
                        }
                    },
                    "cppcheck unittests": {
                        dir('outpost-core') {
                            sh 'make cppcheck-unittests'
                        }
                    }
                )
            }
       }
       stage("Check for Compiler warnings") {
            steps {
                dir('outpost-core') {
                    publishCppcheck pattern: "build/cppcheck/*.xml"

                    recordIssues blameDisabled: true,
                        tools: [
                            gcc4(), clang()
                        ],
                        qualityGates: [[threshold: 1, type: 'TOTAL', unstable: true]]
                    recordIssues blameDisabled: true,
                        tools: [
                            cppCheck(pattern: "build/cppcheck/*.xml")
                        ],
                        qualityGates: [[threshold: 1, type: 'TOTAL_ERROR', unstable: false],
                                       [threshold: 1, type: 'TOTAL_HIGH', unstable: false],
                                       [threshold: 3, type: 'TOTAL_NORMAL', unstable: true]] // TODO reduce threshold when sources have been cleaned
                }
            }
        }
    }

    post {
        failure {
            updateGitlabCommitStatus name: 'build', state: 'failed'
        }
        unstable {
            updateGitlabCommitStatus name: 'build', state: 'failed'
        }
        success {
            updateGitlabCommitStatus name: 'build', state: 'success'
        }
        always {
            archiveArtifacts artifacts: '**/build/cppcheck/**/*'
        }
    }
}
