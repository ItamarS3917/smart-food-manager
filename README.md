# Smart Family Food Management App

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Test Coverage](https://img.shields.io/badge/coverage-95%25-brightgreen.svg)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)]()

A comprehensive solution for modern families to plan meals, optimize shopping, and reduce food waste. Built with performance and usability in mind, combining Python's flexibility with C++'s efficiency.

## 🌟 Features

### Core Functionality
- **Intelligent Meal Planning**
  - Recipe management with nutritional information
  - Dynamic meal scheduling
  - Dietary preferences and restrictions support
  - Smart serving size calculations

- **Shopping Optimization**
  - Automated shopping list generation
  - Cost optimization algorithms
  - Store-specific price tracking
  - Bulk purchase recommendations

- **Waste Reduction**
  - Ingredient expiry tracking
  - Usage pattern analysis
  - Waste statistics and reporting
  - Smart storage recommendations

### Technical Highlights
- High-performance C++ core engine
- Intuitive PyQt6 desktop interface
- RESTful FastAPI backend
- Real-time data synchronization
- Efficient PostgreSQL data storage
- Redis caching for optimal performance

## 🚀 Getting Started

### Prerequisites
- Python 3.8+
- C++17 compatible compiler
- PostgreSQL 13+
- Redis 6+
- Docker and Docker Compose
- Git

### Development Environment Setup

1. Clone the repository:
```bash
git clone https://github.com/ItamarS3917/smart-food-manager.git
cd smart-food-manager
```

2. Install dependencies:
```bash
./scripts/setup/install_dependencies.sh
```

3. Configure development environment:
```bash
./scripts/setup/setup_dev_env.sh
```

4. Initialize database:
```bash
./scripts/setup/setup_database.sh
```

### Building the Application

1. Build C++ core:
```bash
./scripts/build/build_cpp.sh
```

2. Build desktop application:
```bash
./scripts/build/build_frontend.sh
```

### Running Tests

```bash
# Run C++ tests
cd cpp/build && ctest

# Run Python backend tests
pytest backend/tests

# Run frontend tests
pytest frontend/tests
```

## 📐 Architecture

### Component Overview
- **C++ Core Engine** (`cpp/`)
  - Optimized data structures
  - Advanced algorithms
  - Python bindings via pybind11
  - Memory-efficient storage patterns

- **Python Backend** (`backend/`)
  - FastAPI application
  - SQLAlchemy ORM
  - Redis caching
  - Background task processing

- **Desktop Frontend** (`frontend/`)
  - PyQt6 GUI
  - Responsive layouts
  - Real-time updates
  - Offline capability

## 🏗️ Project Structure

```
smart-food-manager/
├── cpp/               # C++ core engine
├── backend/          # FastAPI backend
├── frontend/         # PyQt6 desktop app
├── tests/            # Test suites
├── docs/             # Documentation
└── scripts/          # Build and deployment scripts
```

For detailed structure information, see [Project Structure](docs/architecture/overview.md).

## 💻 Development

### Code Style
- C++: Follow modern C++17 guidelines
- Python: PEP 8 compliance
- Comprehensive documentation required
- Unit tests for all new features

### Branch Strategy
- `main`: Production-ready code
- `develop`: Integration branch
- `feature/*`: New features
- `bugfix/*`: Bug fixes
- `release/*`: Release preparation

### Commit Messages
Follow conventional commits specification:
- `feat:` New features
- `fix:` Bug fixes
- `docs:` Documentation
- `style:` Code style changes
- `refactor:` Code refactoring
- `test:` Test updates
- `chore:` Maintenance tasks

## 📈 Performance Metrics

- API response times < 100ms
- GUI responsiveness < 16ms
- Memory usage < 100MB
- Database query times < 50ms
- Test coverage > 90%

## 🛡️ Security

- JWT authentication
- Role-based access control
- Input validation
- SQL injection prevention
- XSS protection
- Rate limiting
- Regular security audits

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'feat: Add amazing feature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📜 License

[MIT License](LICENSE) - see the LICENSE file for details.

## 🙏 Acknowledgments

- [FastAPI](https://fastapi.tiangolo.com/)
- [PyQt](https://www.riverbankcomputing.com/software/pyqt/)
- [SQLAlchemy](https://www.sqlalchemy.org/)
- [pybind11](https://github.com/pybind/pybind11)
- [nlohmann/json](https://github.com/nlohmann/json)

---
Built with ❤️ for families who love good food and hate waste.
