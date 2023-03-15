#ifndef MAINWINDOW_H
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkNamedColors.h>
#include <vtkUnstructuredGrid.h>
#include <QMainWindow>
#include <vtkRenderer.h> 
#include<vtkPolyDataMapper.h>
#include<vector>
#include<string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer> mRenderer;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<QVTKInteractor> mInteractor;
    vtkNew<vtkInteractorStyleTrackballActor> mInteractorStyle;

public slots:
        void generalFunc();
        void onChangedBasicColorIndex(int);
        void onChangedOpacityValue(int);
        void selectType();
        void onChangedColor(int);
        void getcolors();  
        void deleteShape();
        void displayActors();
};

#endif // MAINWINDOW_H
