//
// Created by liyh on 2019/9/11.
//

#ifndef TRYGL_PROGRAM_H
#define TRYGL_PROGRAM_H


class Program {
public:
	Program();

	~Program();

	bool Build(const char *vertexSource, const char *fragmentSource,
			   const std::vector<std::string> &attribLocationNames, std::vector<GLint> &attribLocations,
			   const std::vector<std::string> &uniformLocationNames, std::vector<GLint> &uniformLocations);

	struct Attribute {
		GLuint location;				//
		GLint componentCountPerItem;	// 属性数组中每项的数组成
		GLint componentDataType;		// 项的数据类型
		GLsizei stride;					// 每项间隔字节
		const void *pointer;			// CPU buffer 或 VBO 的数据偏移
		class Buffer * buffer;			// VBO
	};
	struct Uniform {
	};

	void Use(const std::vector<const Attribute *> * attribData, const std::vector<const Uniform *> * uniformData);

private:
	void clean();

	std::string m_error;
	GLuint m_program;
};


#endif //TRYGL_PROGRAM_H
