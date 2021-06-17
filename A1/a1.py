if __name__ == '__main__':
    # V = set()
    # E = set()


    def addAStreet(centralDict, parsedInputCommand):
        verticesList = []
        for vertex in (parsedInputCommand[2:]):
            xCoordinate = int(vertex.split(",")[0][1:])
            yCoordinate = int(vertex.split(",")[1][:len(vertex.split(",")[1]) - 1])
            verticesList.append((xCoordinate, yCoordinate))
        centralDict[parsedInputCommand[1][1:len(parsedInputCommand[1]) - 1]] = verticesList


    def changeAStreet(centralDict, parsedInputCommand):
        verticesList = []
        for vertex in (parsedInputCommand[2:]):
            xCoordinate = int(vertex.split(",")[0][1:])
            yCoordinate = int(vertex.split(",")[1][:len(vertex.split(",")[1]) - 1])
            verticesList.append((xCoordinate, yCoordinate))
        centralDict[parsedInputCommand[1][1:len(parsedInputCommand[1]) - 1]] = verticesList


    def outputGraph(centralDict):
        V = set()
        E = set()
        def segmentIntersection(segment1, segment2):  # segment1 or segment2 format ((2,2),(4,2)): tuples in tuple
            # def get_intersection(segment_1, segment_2):
            x1 = segment1[0][0]
            y1 = segment1[0][1]
            x2 = segment1[1][0]
            y2 = segment1[1][1]
            x3 = segment2[0][0]
            y3 = segment2[0][1]
            x4 = segment2[1][0]
            y4 = segment2[1][1]
            try:
                p1 = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / (
                        (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
                q1 = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / (
                        (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
            except ZeroDivisionError:
                return None
            if max(min(x1, x2), min(x3, x4)) <= p1 <= min(max(x1, x2), max(x3, x4)) and max(min(y1, y2),
                                                                                            min(y3, y4)) <= q1 <= min(
                    max(y1, y2), max(y3, y4)):
                # return "{:.2f}".format(p1),q1
                # return "({0:.2f},{1:.2f})".format(p1, q1)
                return p1, q1  # return a tuple
            return None

        # get all segments of each street from the "central data structure"
        def allSegmentsOfEachStreet(centralDict):
            dict = {}
            for key in centralDict.keys():
                list = []
                for index in range(len(centralDict.get(key)) - 1):
                    list.append((centralDict.get(key)[index], centralDict.get(key)[index + 1]))
                dict[key] = list
            return dict

        def findAllIntersectionsAndGenerateGraph(dict):

            for i in range(len(dict) - 1):
                for j in range(i + 1, len(dict)):
                    for line1 in dict.get(list(dict.keys())[i]):
                        for line2 in dict.get(list(dict.keys())[j]):
                            intersection = segmentIntersection(line1, line2)  # intersection format: (2,2)
                            # print(intersection)
                            if intersection is not None:
                                # put intersections into set V, and put another end-point of the segment contains the intersection into set V
                                V.add(intersection)
                                V.add(line1[0])
                                V.add(line1[1])
                                V.add(line2[0])
                                V.add(line2[1])
                                # put the segment in previous step (format:((2,2),(4,2)): two tuples in a tuple) in set E )
                                E.add((intersection, line1[0]))
                                E.add((intersection, line1[1]))
                                E.add((intersection, line2[0]))
                                E.add((intersection, line2[1]))
            return V,E

        def VToDisplayedV(V):
            displayedV = set()
            for tuple in V:
                x = float("{:.1f}".format(tuple[0]))
                y = float("{:.1f}".format(tuple[1]))
                displayedV.add((x, y))
            return displayedV

        def EToDisplayedE(E):
            displayedE = set()
            for tuplePair in E:
                t = []
                for tu in tuplePair:
                    x = float("{:.1f}".format(tu[0]))
                    y = float("{:.1f}".format(tu[1]))
                    t.append((x, y))
                t = tuple(t)
                displayedE.add((t))
            return displayedE

        def isPointinLine(point, line):  # if the point is  one of the end-points of the line, return false
            # point format: (3,2)
            # line format: ((4, 4), (5, 5)))
            try:
                (point[1] - line[1][1]) / (
                        line[0][1] - line[1][1]) == (
                        point[0] - line[1][0]) / (
                        line[0][0] - line[1][0]) and min(line[0][0], line[1][0]) < point[0] < max(line[0][0],
                                                                                                  line[1][0])
            except ZeroDivisionError:
                if (line[0][0] == line[1][0] and line[1][0] == point[0]) and min(
                        line[0][1], line[1][1]) < point[1] < max(
                    line[0][1], line[1][1]):
                    return True
                if (line[0][1] == line[1][1] and line[1][1] == point[1]) and min(line[0][0], line[1][0]) < point[
                    0] < max(
                        line[0][0], line[1][0]):
                    return True
                return False
            else:
                if (point[1] - line[1][1]) / (
                        line[0][1] - line[1][1]) == (
                        point[0] - line[1][0]) / (
                        line[0][0] - line[1][0]) and min(line[0][0], line[1][0]) < point[0] < max(
                    line[0][0], line[1][0]):
                    return True
                else:
                    return False
            return

        def deleteUnqualifiedEdge(E):
            VCopy = V.copy()
            ECopy = E.copy()
            for point in VCopy:
                for edge in ECopy:
                    if isPointinLine(point, edge):
                        # delete edge from E
                        E.remove(edge)
                        # add two segments(divided by point) to E
                        E.add((point, edge[0]))
                        E.add((point, edge[1]))
            return E

        def isSameLine(line1, line2):
            if line1[0] == line2[1] and line1[1] == line2[0]:
                return True
            return False

        def deleteDuplicatedEdge(E):
            # input {((4.0, 4.0), (4.0, 7.0)),((4.0, 7.0), (4.0, 4.0))}
            # output {((4.0, 7.0), (4.0, 4.0))}
            EList = list(E)
            for i in range(len(E) - 1):
                for j in range(i + 1, len(E)):
                    if isSameLine(EList[i], EList[j]):
                        E.remove(EList[i])
            return E

        findAllIntersectionsAndGenerateGraph(allSegmentsOfEachStreet(centralDict))
        print(VToDisplayedV(V))
        print(len(VToDisplayedV(V)))
        finalE = deleteDuplicatedEdge(deleteUnqualifiedEdge(EToDisplayedE(E)))
        print(finalE)
        print(len(finalE))


    def removeAStreet(centralDict, parsedInputCommand):
        # print(parsedInputCommand)
        # print(type(parsedInputCommand[1]))
        centralDict.pop(parsedInputCommand[1][1:-1], None)
        return centralDict

    inputCommand = input("Input a command, \"x\" for existing: ")
    # parse inputCommand
    parsedInputCommand = inputCommand.split(" ")
    commandType = parsedInputCommand[0]
    centralDict = {}
    while commandType != "x":

        if commandType == "a":
            # add a street
            addAStreet(centralDict, parsedInputCommand)
            print(centralDict)
        elif commandType == "c":
            # change a street
            changeAStreet(centralDict, parsedInputCommand)
            print(centralDict)
        elif commandType == "r":
            # remove a street"
            removeAStreet(centralDict,parsedInputCommand)
            print(centralDict)
        elif commandType == "g":
            # output graph as the result
            outputGraph(centralDict)
        else:
            print("Wrong command. Input a command again.")
        inputCommand = input()
        parsedInputCommand = inputCommand.split(" ")
        commandType = parsedInputCommand[0]

