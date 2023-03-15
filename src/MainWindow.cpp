#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
#include <MainWindow.h>
#include <ui_MainWindow.h>
#include<vtkSphereSource.h>
#include<vtkConeSource.h>
#include<QMessageBox.h>
#include<vtkActor.h>
#include <vtkNamedColors.h>
#include<vtkCylinderSource.h>
#include <vtkProperty.h>
#include<QComboBox.h>
#include<QSpinBox.h>
#include<vtkPolyDataAlgorithm.h>
#include<vtkCubeSource.h>
#include<vtkDiskSource.h>
#include<vtkPolyDataMapper.h>
#include<string>
#include<vector>
#include<stdio.h>
#include <vtkColor.h>
using namespace std;
const int color[5] = {7,1,2,4,0};
vector<vtkSmartPointer<vtkActor>> actors;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
    mRenderer(vtkSmartPointer<vtkRenderer>::New()),
    mInteractor(vtkSmartPointer<QVTKInteractor>::New())
{
    ui->setupUi(this);
    this->setWindowTitle("3D Painter");
    getcolors();
    
    mRenderWindow->AddRenderer(mRenderer);
    mRenderWindow->SetInteractor(mInteractor);
    ui->openGLWidget->setRenderWindow(mRenderWindow);
    mInteractor->SetInteractorStyle(mInteractorStyle);
    mInteractor->Initialize();
    // set the background color 
    mRenderer->SetBackground(0.2, 0.2, 0.4);

    // actions 
    QObject::connect(ui->changeColor_comboBox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onChangedBasicColorIndex);

    QObject::connect(ui->changeOpacity_spinBox,
        QOverload<int>::of(&QSpinBox::valueChanged),
        this, &MainWindow::onChangedOpacityValue);
    
        QObject::connect(ui->shapes_ComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::selectType);

        QObject::connect(ui->colors_ComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onChangedColor);

        QObject::connect(ui->deleteShape_button,
                &QPushButton::clicked,
            this, &MainWindow::deleteShape);

}
void MainWindow::selectType() {
    string s = ui->shapes_ComboBox->currentText().toStdString();
    if (s == "Cone") {
        vtkNew<vtkConeSource> cone;
        cone->SetResolution(100);
        
        cone->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(cone->GetOutput());
        map->SetObjectName("Cone");
        mapper = map;
        generalFunc();
    }
    else if (s == "Sphere") {
        vtkNew<vtkSphereSource> sphere;       
        sphere->SetThetaResolution(80);
       
        sphere->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(sphere->GetOutput());
        map->SetObjectName("Sphere");
        mapper = map;
        generalFunc();
    }
    else if (s == "Cube") {
        vtkNew<vtkCubeSource> cube;
       
        cube->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(cube->GetOutput());
        map->SetObjectName("Cube");
        mapper = map;
        generalFunc();
    }
    else if (s == "Cylinder") {
        vtkNew<vtkCylinderSource> cylinder;
        cylinder->SetResolution(80);
        cylinder->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(cylinder->GetOutput());
        map->SetObjectName("Cylinder");
        mapper = map;
        generalFunc();
    }
    else if (s == "Pyramid") {
        vtkNew<vtkConeSource> pyramid;

        pyramid->SetResolution(4);
        pyramid->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(pyramid->GetOutput());
        map->SetObjectName("Pyramid");
        mapper = map;
        generalFunc();
    }
    else if (s == "Hemisphere") {
        vtkNew<vtkSphereSource> hemisphere;
        hemisphere->SetThetaResolution(80);
        hemisphere->SetPhiResolution(80);
        hemisphere->SetEndTheta(180);
        hemisphere->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(hemisphere->GetOutput());
        map->SetObjectName("Hemisphere");
        mapper = map;
        generalFunc();
    }
    else if (s == "Tube") {
        vtkNew<vtkCylinderSource> tube;
        tube->SetResolution(80);
        tube->SetCapping(false);
        tube->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(tube->GetOutput());
        map->SetObjectName("Tube");
        mapper = map;
        generalFunc();
    }
    else if (s == "Doughnut") {
        vtkNew<vtkDiskSource> Doughnut;
        Doughnut->SetCircumferentialResolution(100);
        Doughnut->Update();
        vtkSmartPointer<vtkPolyDataMapper> map =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        map->SetInputData(Doughnut->GetOutput());
        map->SetObjectName("Doughnut");
        mapper = map;
        generalFunc();
    }
}
void MainWindow::generalFunc() {

     vtkSmartPointer<vtkActor> shape
         = vtkSmartPointer<vtkActor>::New();
     shape->SetMapper(mapper);
     string s=mapper->GetObjectName();
     shape->SetObjectName(s);
     actors.push_back(shape);
     string name = to_string(actors.size()) + " " + actors[actors.size() - 1]->GetObjectName();
     ui->myShapes_list->addItem(name.c_str());
     mRenderer->AddViewProp(shape);
     mRenderer->ResetCamera();
     mRenderWindow->Render();
}
void MainWindow::onChangedBasicColorIndex(int index) {
    int num = ui->myShapes_list->currentRow();
    if (num == -1) {
        QMessageBox::warning(this, " ", "Please Select an item ..!");
        ui->colors_ComboBox->setCurrentIndex(0);
    }
    else {
    int clr=color[index];
    //convert mapper to actor
    vtkSmartPointer<vtkProperty> prop =
        vtkSmartPointer<vtkProperty>::New();
    int d1, d2, d3;
    d1 = clr % 2;
    clr /= 2;
    d2 = clr % 2;
    clr /= 2;
    d3 = clr % 2;
    vtkSmartPointer<vtkActor> act
        = vtkSmartPointer<vtkActor>::New();
         act = actors[num];
        prop = act->GetProperty();
        prop->SetColor(d1, d2, d3);
        act->SetProperty(prop);
        mRenderWindow->Render();
    }
}
void MainWindow::onChangedOpacityValue(int val) {
    int num = ui->myShapes_list->currentRow();
    if (num == -1) {
        QMessageBox::warning(this, " ", "Please Select an item ..!");
        ui->colors_ComboBox->setCurrentIndex(0);
    }
    else {
        vtkSmartPointer<vtkProperty> prop =
            vtkSmartPointer<vtkProperty>::New();
        // QMessageBox::warning(this," ",to_string(val/100.0).c_str());

        vtkSmartPointer<vtkActor> act
            = vtkSmartPointer<vtkActor>::New();
        act = actors[num];
        prop = act->GetProperty();
        prop->SetOpacity(val / 100.0);
        act->SetProperty(prop);
        mRenderWindow->Render();
    }
}
void MainWindow::onChangedColor(int ind) {
    int num = ui->myShapes_list->currentRow();
    if (num == -1) {
        QMessageBox::warning(this, " ", "Please Select an item ..!");
        ui->colors_ComboBox->setCurrentIndex(0);
    }
    else
    {
        vtkSmartPointer<vtkActor> act
            = vtkSmartPointer<vtkActor>::New();
        act = actors[num];
        string s = ui->colors_ComboBox->currentText().toStdString();
        vtkNew<vtkNamedColors> namedColors;
        act->GetProperty()->SetColor(namedColors->GetColor4d(s).GetData());
        mRenderWindow->Render();
    }
}
void MainWindow::getcolors() {
    vtkNew<vtkNamedColors> colors;
    string s=colors->GetColorNames();
    string m = "";
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != '\n')
        {
            m += s[i];
        }
        else
        {
            ui->colors_ComboBox->addItem(m.c_str());
            m = "";
        }
    }
}
void MainWindow::deleteShape() {
    int num = ui->myShapes_list->currentRow();
    if (num == -1) {
        QMessageBox::warning(this, " ", "Please Select an item to delete..!");
    }
    else {
        vtkSmartPointer<vtkActor> act
            = vtkSmartPointer<vtkActor>::New();
        act = actors[num];
        mRenderer->RemoveActor(act);
        actors.erase(actors.begin() + num);
        ui->myShapes_list->clear();
        displayActors();
        mRenderWindow->Render();
    }
}
void MainWindow::displayActors() {
    for (int i = 0; i < actors.size(); i++)
    {
        int num = i + 1;
        string name =
             to_string(num)+ " " + actors[i]->GetObjectName();
        ui->myShapes_list->addItem(name.c_str());
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

